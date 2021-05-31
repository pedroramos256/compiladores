#include <string>
#include <stack>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include "ast/all.h"  // all.h is automatically generated
// must come after other #includes
#include "fir_parser.tab.h"

//---------------------------------------------------------------------------

void fir::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
void fir::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value()); // integer literal is on the stack: push an integer
  } else {
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  }
}
void fir::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value()); // load number to the stack
  } else {
    _pf.SDOUBLE(node->value());    // double is on the DATA segment
  }
}

void fir::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;
  /* generate the string literal */
  _pf.RODATA(); // strings are readonly DATA
  _pf.ALIGN(); // make sure the address is aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters
  if (_inFunctionBody || _inFunctionArgs) {
    // local variable initializer
    _pf.TEXT();
    _pf.ALIGN(); // make sure the address is aligned
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.ALIGN(); // make sure the address is aligned
    _pf.SADDR(mklbl(lbl1));
  }
}

void fir::postfix_writer::do_null_node(fir::null_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; // a pointer is a 32-bit integer
  if (_inFunctionBody) {
    _pf.INT(0);
  } else {
    _pf.SINT(0);
  }
}


//---------------------------------------------------------------------------

void fir::postfix_writer::do_identity_node(fir::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}
void fir::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

void fir::postfix_writer::do_sizeof_node(fir::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->expression()->type()->size());
}

void fir::postfix_writer::do_address_of_node(fir::address_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT))
    _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    size_t typesize = cdk::reference_type::cast(node->type())->referenced()->size();
    _pf.INT(typesize);
    _pf.MUL();
  }

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
    _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    size_t typesize = cdk::reference_type::cast(node->type())->referenced()->size();
    _pf.INT(typesize);
    _pf.MUL();
  }

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DADD();
  else
    _pf.ADD();
}
void fir::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT))
    _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    size_t typesize = cdk::reference_type::cast(node->type())->referenced()->size();
    _pf.INT(typesize);
    _pf.MUL();
  }

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
    _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    size_t typesize = cdk::reference_type::cast(node->type())->referenced()->size();
    _pf.INT(typesize);
    _pf.MUL();
  }

  // sub pointers
  if (node->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)) {
      // os() << "        ;; sub pointers " << std::endl;
    // lType and rType endup being the same, checked in typechecker
    size_t typesize = cdk::reference_type::cast(node->left()->type())->referenced()->size();
    _pf.SUB();
    _pf.INT(typesize);
    _pf.DIV();

/* 
    std::shared_ptr<cdk::basic_type> lType;
    lType = node->left()->type();

    while (lType->name() == cdk::TYPE_POINTER)
      lType = cdk::reference_type::cast(lType)->referenced();

    _pf.SUB();
    _pf.INT(lType->size());
    _pf.DIV();

    std::cerr << "pf sub pointers" << std::endl;  */

  } else if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DSUB();
  else
    _pf.SUB();
}

void fir::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
    _pf.I2D();

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DMUL();
  else
    _pf.MUL();
}
void fir::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
    _pf.I2D();

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DDIV();
  else
    _pf.DIV();
}
void fir::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

//---------------------------------------------------------------------------
//protected
void fir::postfix_writer::do_ScalarLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
    _pf.I2D();
  
  if(node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }
}

void fir::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
  _pf.LT();
}
void fir::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
  _pf.LE();
}
void fir::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
  _pf.GE();
}
void fir::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
  _pf.GT();
}

//---------------------------------------------------------------------------
//protected
void fir::postfix_writer::do_GeneralLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
    _pf.I2D();
  
  if(node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }
}

void fir::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_GeneralLogicalExpression(node, lvl);
  _pf.NE();
}
void fir::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_GeneralLogicalExpression(node, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  auto symbol = _symtab.find(id);   // after safe expression, will always find
  if (_inFunctionBody && _function && _function==symbol) {    // object that contains return value offset is the function itself
    _pf.LOCAL(symbol->return_value_offset());
  } else if (symbol->global()) {
    _pf.ADDR(symbol->name());
  } else {
    _pf.LOCAL(symbol->offset());
  }
}

void fir::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDDOUBLE();
  } else {
    // integers, pointers, and strings
    _pf.LDINT();
  }
}

void fir::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl + 2); // determine the new value

  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    if (node->rvalue()->type()->name() == cdk::TYPE_INT) _pf.I2D();
    _pf.DUP64(); 
  } else {
    _pf.DUP32(); 
  }

  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}

void fir::postfix_writer::do_stack_alloc_node(fir::stack_alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  if(cdk::reference_type::cast(node->type())->referenced()->name() == cdk::TYPE_DOUBLE)
    _pf.INT(3);
  else
    _pf.INT(2);

  _pf.SHTL();
  _pf.ALLOC();    
  _pf.SP();
}

void fir::postfix_writer::do_index_node(fir::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->base()->accept(this, lvl);
  node->index()->accept(this, lvl);
  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD(); 
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_function_definition_node(fir::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFunctionBody || _inFunctionArgs ) {
    error(node->lineno(), "cannot define function in body or in arguments");
    return;
  }

  // remember symbol so that args and body know
  _function = new_symbol();
  _functions_to_declare.erase(_function->name());  // just in case
  reset_new_symbol();

  _currentBodyRetLabel = mklbl(++_lbl);

  _offset = 8; // prepare for arguments (4: remember to account for return address)
  _symtab.push(); // scope of args
  if(node->arguments()) {
    _inFunctionArgs = true;
    node->arguments()->accept(this, lvl + 2);
    _inFunctionArgs = false;
  }

  _pf.TEXT();
  _pf.ALIGN();
  if (node->qualifier() == tPUBLIC) _pf.GLOBAL(_function->name(), _pf.FUNC());
  _pf.LABEL(_function->name());

  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab, _function);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables */
  
  _inFunctionBody = true;

  // load default return value to stack (ret value, can be changed later inside function)
  if (!_function->is_typed(cdk::TYPE_VOID) && node->return_value()){
    node->return_value()->accept(this, lvl + 2);
    if(_function->is_typed(cdk::TYPE_INT) || _function->is_typed(cdk::TYPE_STRING)
      || _function->is_typed(cdk::TYPE_POINTER)){
      _pf.LOCAL(_function->return_value_offset());
      _pf.STINT();
    }
    else if (_function->is_typed(cdk::TYPE_DOUBLE)){
      _pf.I2D();
      _pf.LOCAL(_function->return_value_offset());
      _pf.STDOUBLE();
    }
    else throw std::string("Wrong return value type."); // should never happen
  }

  _offset = _function->return_value_offset(); // prepare for local variable

  os() << "        ;; before function PBE " << std::endl;
  if (node->prologue()) {
    os() << "        ;; before prologue " << std::endl;
    node->prologue()->accept(this, lvl + 4);
    os() << "        ;; after prologue " << std::endl;
  }
  if (node->body()) {
    os() << "        ;; before body " << std::endl;
    node->body()->accept(this, lvl + 4);
    os() << "        ;; after body " << std::endl;
  }

  _pf.LABEL(_currentBodyRetLabel);    // when existing, epilogue is always runned at function leave

  if (node->epilogue()) {
    os() << "        ;; before epilogue " << std::endl;
    node->epilogue()->accept(this, lvl + 4);
    os() << "        ;; after epilogue " << std::endl;
  }
  
  os() << "        ;; after function PBE " << std::endl;
  _inFunctionBody = false;


  if (!_function->is_typed(cdk::TYPE_VOID)) {
    // load return value to register
    _pf.LOCAL(_function->return_value_offset());
    
    if (_function->is_typed(cdk::TYPE_INT) || _function->is_typed(cdk::TYPE_STRING)
        || _function->is_typed(cdk::TYPE_POINTER)) {
      _pf.LDINT();
      _pf.STFVAL32();
      
    } else if (_function->is_typed(cdk::TYPE_DOUBLE)) {
      _pf.LDDOUBLE();
      _pf.STFVAL64();
    } 
  }
  
  _pf.LEAVE();
  _pf.RET();

  _symtab.pop(); // scope of arguments

  // declare external functions
  if (node->identifier() == "fir") {
    for (std::string fun : _functions_to_declare)
      _pf.EXTERN(fun);

    for (std::string var : _variables_to_declare)
      _pf.EXTERN(var);
  }
}

void fir::postfix_writer::do_function_declaration_node(fir::function_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot declare function in body or in args");
    return;
  }
  
  if (!new_symbol()) return;

  auto function = new_symbol();
  _functions_to_declare.insert(function->name());
  reset_new_symbol();
}

void fir::postfix_writer::do_variable_declaration_node(fir::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifier();

  std::cout << "INITIAL OFFSET: " << _offset << std::endl;

  // type size?
  int offset = 0, typesize = node->type()->size(); // in bytes
  std::cout << "ARG: " << id << ", " << typesize << std::endl;
  if (_inFunctionBody) {
    std::cout << "IN BODY" << std::endl;
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    std::cout << "IN ARGS" << std::endl;
    offset = _offset;
    _offset += typesize;
  } else {
    std::cout << "GLOBAL!" << std::endl;
    offset = 0; // global variable
  }
  std::cout << "OFFSET: " << id << ", " << offset << std::endl;

  auto symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionBody) {
    // local variables
    if (_function && id == _function->name())
      error(node->lineno(), "Cannot declare variable inside a function conflicting with return value variable");

    else if (node->qualifier() != tPRIVATE) error(node->lineno(), "Cannot declare public or external variables inside a function");
    else if (node->initializer()) {
      node->initializer()->accept(this, lvl);
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER)) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
        if (node->initializer()->is_typed(cdk::TYPE_INT))
          _pf.I2D();
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }
  } 
  else if (_inFunctionArgs) {
    if (node->qualifier() != tPRIVATE) throw std::string("Cannot declare public or external variables inside a function arguments");
    else if (node->initializer()) throw std::string("Cannot declare variables with default values inside a function arguments");
  }
  else {
    if (node->qualifier() == tEXTERNAL) {
      _variables_to_declare.insert(symbol->name());
      return;
    }
    else if (node->initializer() == nullptr) {
      _pf.BSS();
      _pf.ALIGN();
      if (node->qualifier() == tPUBLIC) _pf.GLOBAL(symbol->name(), _pf.OBJ());
      _pf.LABEL(id);
      _pf.SALLOC(typesize);
    }
    else {
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER)) {
        _pf.DATA();
        _pf.ALIGN();
        if (node->qualifier() == tPUBLIC) _pf.GLOBAL(symbol->name(), _pf.OBJ());
        _pf.LABEL(id);

        if (node->is_typed(cdk::TYPE_INT)) {
          node->initializer()->accept(this, lvl);
        } else if (node->is_typed(cdk::TYPE_POINTER)) {
          node->initializer()->accept(this, lvl);
        } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
          if (node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
            node->initializer()->accept(this, lvl);
          } else if (node->initializer()->is_typed(cdk::TYPE_INT)) {
            cdk::integer_node *dclini = dynamic_cast<cdk::integer_node*>(node->initializer());
            cdk::double_node ddi(dclini->lineno(), dclini->value());
            ddi.accept(this, lvl);
          } else {
            error(node->lineno(), id, "has bad initializer for real value");
          }
        }

      } else if (node->is_typed(cdk::TYPE_STRING)) {
        _pf.DATA();
        _pf.ALIGN();
        if (node->qualifier() == tPUBLIC) _pf.GLOBAL(symbol->name(), _pf.OBJ());
        _pf.LABEL(id);
        node->initializer()->accept(this, lvl);
      } else {
        error(node->lineno(), id, "has unexpected initializer");
      }
    }
  }

}

void fir::postfix_writer::do_block_node(fir::block_node * const node, int lvl) {
  _symtab.push(); // for block-local vars
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

void fir::postfix_writer::do_prologue_node(fir::prologue_node * const node, int lvl) {
  // uses var environment created inside function_definition_node
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
}


//---------------------------------------------------------------------------

void fir::postfix_writer::do_evaluation_node(fir::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.TRASH(node->argument()->type()->size());
}

void fir::postfix_writer::do_function_call_node(fir::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto symbol = _symtab.find(node->identifier());

  size_t argsSize = 0;
  if (node->arguments() && node->arguments()->size() > 0) {
    for (int ax = node->arguments()->size() - 1; ax >= 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax));
      arg->accept(this, lvl + 2);
      if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && arg->is_typed(cdk::TYPE_INT)) {
        _pf.I2D();
      }
      argsSize += symbol->argument_size(ax);
    }
  }
  _pf.CALL(node->identifier());
  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (symbol->is_typed(cdk::TYPE_INT) || symbol->is_typed(cdk::TYPE_POINTER) || symbol->is_typed(cdk::TYPE_STRING)) {
    _pf.LDFVAL32();
  } else if (symbol->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDFVAL64();
  } else {
    // cannot happen!
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_read_node(fir::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _functions_to_declare.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } else if (node->is_typed(cdk::TYPE_INT)) {
    _functions_to_declare.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  } else
    error(node->lineno(), "cannot read type");
}

void fir::postfix_writer::do_write_node(fir::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
    auto child = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ix));

    child->accept(this, lvl); // expression to print
    if (child->is_typed(cdk::TYPE_INT)) {
      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4); // delete the printed value
    } else if (child->is_typed(cdk::TYPE_DOUBLE)) {
      _functions_to_declare.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8); // delete the printed value
    } else if (child->is_typed(cdk::TYPE_STRING)) {
      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4); // delete the printed value's address
    } else {
      std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
      exit(1);
    }
  }
  if(node->newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println"); // print a newline
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_while_node(fir::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _whileCond.push(++_lbl); // before condition
  _whileEnd.push(++_lbl); // after while (may have finally block)

  os() << "        ;; WHILE condition" << std::endl;

  _pf.ALIGN();
  _pf.LABEL(mklbl(_whileCond.top()));
  node->condition()->accept(this, lvl + 2);
  _pf.JZ(mklbl(_whileEnd.top()));

  os() << "        ;; WHILE block" << std::endl;
  node->block()->accept(this, lvl + 2);

  _pf.JMP(mklbl(_whileCond.top()));   // reavaliate condition


  _pf.LABEL(mklbl(_whileEnd.top()));
  // popped before finally to prevent using break for the same while
  _whileCond.pop();
  _whileEnd.pop();

  if (node->finally_block()) {
    _inFinallyBlock = true;
    os() << "        ;; WHILE finally block" << std::endl;
    node->finally_block()->accept(this, lvl + 2);
    _inFinallyBlock = false;
  }
}

void fir::postfix_writer::do_restart_node(fir::restart_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFinallyBlock) error(node->lineno(), "restart inside finally block");

  if (_whileCond.size() != 0) {
    if ((size_t)node->cycle_number() <= _whileCond.size()) {
      std::stack<int> copyStack;
      copyStack = _whileCond;

      int lbl = 0;
      for (int i=0; i<node->cycle_number(); i++) {
        lbl = copyStack.top();
        copyStack.pop();
      }

      _pf.JMP(mklbl(lbl)); // jump to while condition

    } else 
      error(node->lineno(), "'restart' of more cycles then existing");
  } else
    error(node->lineno(), "'restart' outside 'while'");
}

void fir::postfix_writer::do_leave_node(fir::leave_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFinallyBlock) error(node->lineno(), "leave inside finally block");

  if (_whileEnd.size() != 0) {
    if ((size_t)node->cycle_number() <= _whileEnd.size()) {
      std::stack<int> copyStack;
      copyStack = _whileEnd;

      int lbl = 0;
      for (int i=0; i<node->cycle_number(); i++) {
        lbl = copyStack.top();
        copyStack.pop();
      }

      _pf.JMP(mklbl(lbl)); // jump to while end

    } else 
      error(node->lineno(), "'leave' of more cycles then existing");
  } else
    error(node->lineno(), "'leave' outside 'while'");
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_node(fir::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_else_node(fir::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_return_node(fir::return_node * const node, int lvl) {
  if (_inFunctionBody)
    _pf.JMP(_currentBodyRetLabel);
}
