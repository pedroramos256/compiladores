#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>

// must come after other #includes
#include "fir_parser.tab.h"

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void fir::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------
void fir::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}

void fir::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

void fir::type_checker::do_null_node(fir::null_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_POINTER));
}

//---------------------------------------------------------------------------

void fir::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->argument());
    if (read_node)
      node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  }
  else if (!node->argument()->is_typed(cdk::TYPE_INT) && !node->argument()->is_typed(cdk::TYPE_DOUBLE))
    throw std::string("wrong type in argument of unary expression");

  node->type(node->argument()->type());
}

void fir::type_checker::do_identity_node(fir::identity_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void fir::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void fir::type_checker::do_sizeof_node(fir::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->expression()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_address_of_node(fir::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(cdk::reference_type::create(4, node->lvalue()->type()));
}

//---------------------------------------------------------------------------
//protected:
void fir::type_checker::do_PIDExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(node->left()->type());
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)) {
    node->type(node->right()->type());
  } else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)) {
    std::shared_ptr<cdk::basic_type> lType, rType;
    lType = node->left()->type();
    rType = node->right()->type();

    while (lType->name() == cdk::TYPE_POINTER && rType->name() == cdk::TYPE_POINTER) {
      lType = cdk::reference_type::cast(lType)->referenced();
      rType = cdk::reference_type::cast(rType)->referenced();
    }

    if (lType->name() != rType->name())
      throw std::string("wrong type in binary expression, pointers dont match");

    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)) {    
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->left());
    if (read_node)
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->right());
    if (read_node)
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else {
    throw std::string("wrong types in binary expression");
  }
}

//protected:
void fir::type_checker::do_IDExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->left());
    if (read_node)
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->right());
    if (read_node)
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else {
    throw std::string("wrong types in binary expression");
  }
}

//protected:
void fir::type_checker::do_IntOnlyExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->left());
    if (read_node)
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (!node->left()->is_typed(cdk::TYPE_INT)) 
    throw std::string("integer expression expected in binary operator (left)");
  

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->right());
    if (read_node)
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (!node->right()->is_typed(cdk::TYPE_INT))
    throw std::string("integer expression expected in binary operator (right)");

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  do_PIDExpression(node, lvl);
}
void fir::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  do_PIDExpression(node, lvl);
}
void fir::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  do_IDExpression(node, lvl);
}
void fir::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  do_IDExpression(node, lvl);
}
void fir::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  do_IntOnlyExpression(node, lvl);
}

//---------------------------------------------------------------------------

//protected
void fir::type_checker::do_ScalarLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->left());
    if (read_node)
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if(!node->left()->is_typed(cdk::TYPE_INT) && !node->left()->is_typed(cdk::TYPE_DOUBLE))
    throw std::string("Invalid binary logical expression, expected integer or double (left).");
  
  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->right());
    if (read_node)
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if(!node->right()->is_typed(cdk::TYPE_INT) && !node->right()->is_typed(cdk::TYPE_DOUBLE))
    throw std::string("Invalid binary logical expression, expected integer or double (right).");

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//protected
void fir::type_checker::do_GeneralLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->left());
    if (read_node)
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if(!node->left()->is_typed(cdk::TYPE_INT) && !node->left()->is_typed(cdk::TYPE_DOUBLE)
      && !node->left()->is_typed(cdk::TYPE_POINTER))
    throw std::string("Invalid binary logical expression, expected integer, double or pointer (left).");
  
  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->right());
    if (read_node)
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if(!node->right()->is_typed(cdk::TYPE_INT) && !node->right()->is_typed(cdk::TYPE_DOUBLE)
      && !node->right()->is_typed(cdk::TYPE_POINTER))
    throw std::string("Invalid binary logical expression, expected integer, double or pointer (right).");

  if ((node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE))
    || (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER))
    || (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT))
    || (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
    || (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))) {
      // Accept
  } else {
    throw std::string("Invalid binary logical expression, types dont match");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}


void fir::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}
void fir::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}
void fir::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}
void fir::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}

void fir::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  do_GeneralLogicalExpression(node, lvl);
}
void fir::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  do_GeneralLogicalExpression(node, lvl);
}

//---------------------------------------------------------------------------
//protected:
void fir::type_checker::do_BooleanLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->left());
    if (read_node)
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (!node->left()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->right());
    if (read_node)
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (!node->right()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}
void fir::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}

//---------------------------------------------------------------------------


void fir::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  auto symbol = _symtab.find(id);

  if (symbol) {
    node->type(symbol->type());
  } else {
    throw std::string("undeclared variable '" + id + "'");
  }
}

void fir::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());  
}

void fir::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;
  std::shared_ptr<cdk::basic_type> lType, rType;

  node->lvalue()->accept(this, lvl + 4);
  node->rvalue()->accept(this, lvl + 4);
  
  lType = node->lvalue()->type();
  rType = node->rvalue()->type();

  while (lType->name() == cdk::TYPE_POINTER && rType->name() == cdk::TYPE_POINTER) {
    lType = cdk::reference_type::cast(lType)->referenced();
    rType = cdk::reference_type::cast(rType)->referenced();
  }

  if (rType->name() == cdk::TYPE_UNSPEC) {
    fir::stack_alloc_node *stack_node = dynamic_cast<fir::stack_alloc_node *>(node->rvalue());
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->rvalue());
    
    if (stack_node) { // for stack alloc node
      if (lType->name() == cdk::TYPE_POINTER)
        node->rvalue()->type(node->lvalue()->type());
      else
        throw std::string("pointer expected for assignment of stack alloc address");  
    }
    else if (read_node) { // for read node
      if (lType->name() == cdk::TYPE_INT || lType->name() == cdk::TYPE_DOUBLE)
        node->rvalue()->type(node->lvalue()->type());
      else
        throw std::string("user input assigned to wrong type, can only be assigned to int or double");
    }
  } 
  else if (lType->name() == cdk::TYPE_DOUBLE && rType->name() == cdk::TYPE_INT) {
    // accept
  } else if (lType->name() != rType->name())
    throw std::string("wrong type in assignment, types dont match");

  node->type(node->lvalue()->type());
}

void fir::type_checker::do_stack_alloc_node(fir::stack_alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->argument());
    if (read_node)
      node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in allocation expression");
  }
  auto mytype = cdk::primitive_type::create(0, cdk::TYPE_UNSPEC);
  node->type(mytype);
}

void fir::type_checker::do_index_node(fir::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->base()->accept(this, lvl + 2);
  if (!node->base()->is_typed(cdk::TYPE_POINTER)) throw std::string("pointer expression expected in index left-value");
  std::shared_ptr < cdk::reference_type > btype = cdk::reference_type::cast(node->base()->type());

  node->index()->accept(this, lvl + 2);
  if (node->index()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->index());
    if (read_node)
      node->index()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  } else if (!node->index()->is_typed(cdk::TYPE_INT)) throw std::string("integer expression expected in left-value index");

  node->type(btype->referenced());
}


//---------------------------------------------------------------------------

void fir::type_checker::do_function_definition_node(fir::function_definition_node *const node, int lvl) {
  std::string id;

  // "fix" naming issues...
  if (node->identifier() == "fir")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  auto function = fir::make_symbol(node->qualifier(), node->type(), id, true, true);

  // remember args types
  if (node->arguments()) {
    std::vector < std::shared_ptr < cdk::basic_type >> argtypes;
    for (size_t ax = 0; ax < node->arguments()->size(); ax++)
      argtypes.push_back(node->argument(ax)->type());
    function->set_argument_types(argtypes);
  }
  
  
  if (node->return_value() == nullptr) {
    if (node->is_typed(cdk::TYPE_INT)) {
      node->set_default_return_value_integer();
      node->return_value()->accept(this, lvl);
    }
    else if (node->is_typed(cdk::TYPE_POINTER)) {
      node->set_default_return_value_pointer();
      node->return_value()->accept(this, lvl);
    }
  } else {
    // default return value
    if (node->is_typed(cdk::TYPE_VOID))
      throw std::string("return value specified for void function");

    node->return_value()->accept(this, lvl);
    if (function->type()->name() == node->return_value()->type()->name()
      || (function->is_typed(cdk::TYPE_DOUBLE) && node->return_value()->is_typed(cdk::TYPE_INT))) {
      // ACCEPT
    }
    else
      throw std::string("return value doesnt match function type");
  }


  // Insert object table
  std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->isFunction() && !previous->isInitialized()  // a function declaration
        && ((previous->qualifier() == tPUBLIC && node->qualifier() == tPUBLIC)
            || (previous->qualifier() == tPRIVATE && node->qualifier() == tPRIVATE)
            || (previous->qualifier() == tEXTERNAL && node->qualifier() == tPUBLIC))) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    } else {
      throw std::string("conflicting definition for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

void fir::type_checker::do_function_declaration_node(fir::function_declaration_node *const node, int lvl) {
  std::string id;

  // "fix" naming issues...
  if (node->identifier() == "fir")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  // remember symbol so that args know
  auto function = fir::make_symbol(node->qualifier(), node->type(), id, false, true);

  // remember args types
  if (node->arguments()) {
    std::vector < std::shared_ptr < cdk::basic_type >> argtypes;
    for (size_t ax = 0; ax < node->arguments()->size(); ax++)
      argtypes.push_back(node->argument(ax)->type());
    function->set_argument_types(argtypes);
  }

  std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (!previous->is_argument_types_equal(function->argument_types())) {
      throw std::string("conflicting declaration for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

void fir::type_checker::do_variable_declaration_node(fir::variable_declaration_node *const node, int lvl) {
  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl + 2);

    // resolve unspec
    if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)) {
      fir::stack_alloc_node *stack_node = dynamic_cast<fir::stack_alloc_node *>(node->initializer());
      fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->initializer());
      
      if (stack_node) { // for stack alloc node
        if (node->is_typed(cdk::TYPE_POINTER))
          node->initializer()->type(node->type());
        else
          throw std::string("pointer expected for variable declaration with stack alloc address");  
      }
      else if (read_node) { // for read node
        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE))
          node->initializer()->type(node->type());
        else
          throw std::string("user input for variable declaration of unsupported type, can only be assigned to int or double");
      }
    }

  
    // main case  
    if (node->is_typed(cdk::TYPE_INT)) {
      if (!node->initializer()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type for initializer (integer expected).");
    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
      if (!node->initializer()->is_typed(cdk::TYPE_INT) && !node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
        throw std::string("wrong type for initializer (integer or double expected).");
      }
    } else if (node->is_typed(cdk::TYPE_STRING)) {
      if (!node->initializer()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("wrong type for initializer (string expected).");
      }
    } else if (node->is_typed(cdk::TYPE_POINTER)) {
      // only literal accepted in null node
      if (!node->initializer()->is_typed(cdk::TYPE_POINTER)) {
        throw std::string("wrong type for initializer (pointer expected).");
      } 
    } else {
      throw std::string("unknown type for initializer.");
    }
  }

  const std::string &id = node->identifier();
  auto symbol = fir::make_symbol(node->qualifier(), node->type(), id, (bool)node->initializer(), false);

  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
}

void fir::type_checker::do_block_node(fir::block_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_prologue_node(fir::prologue_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_evaluation_node(fir::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void fir::type_checker::do_function_call_node(fir::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;

  const std::string &id = node->identifier();
  auto symbol = _symtab.find(id);
  if (symbol == nullptr) throw std::string("symbol '" + id + "' is undeclared.");
  if (!symbol->isFunction()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());
  if(node->arguments()){
    if (node->arguments()->size() == symbol->number_of_arguments()) {
      node->arguments()->accept(this, lvl + 4);
      for (size_t ax = 0; ax < node->arguments()->size(); ax++) {
        if (node->argument(ax)->type() == symbol->argument_type(ax)) continue;
        if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && node->argument(ax)->is_typed(cdk::TYPE_INT)) continue;
        if (node->argument(ax)->is_typed(cdk::TYPE_UNSPEC)) { // resolve unspec
          fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->argument(ax));
          if (read_node) { // for read node
            if (symbol->argument_is_typed(ax, cdk::TYPE_INT)) {
              node->argument(ax)->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
              continue;
            }
            else
              throw std::string("user input for function call of unsupported type, can only be matched to int");
          }
        }
        throw std::string("type mismatch for argument " + std::to_string(ax + 1) + " of '" + id + "'.");
      }
    } else {
      throw std::string(
          "number of arguments in call (" + std::to_string(node->arguments()->size()) + ") must match declaration ("
              + std::to_string(symbol->number_of_arguments()) + ").");
    }
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::do_read_node(fir::read_node *const node, int lvl) {
  ASSERT_UNSPEC;
  auto mytype = cdk::primitive_type::create(0, cdk::TYPE_UNSPEC);
  node->type(mytype);
}

void fir::type_checker::do_write_node(fir::write_node *const node, int lvl) {
  node->arguments()->accept(this, lvl);
  
  for(size_t ix = 0; ix < node->arguments()->size(); ix++) {
    cdk::expression_node *expr = dynamic_cast<cdk::expression_node *>(node->arguments()->node(ix));
    if (expr) {
      if (expr->is_typed(cdk::TYPE_UNSPEC)) { // for read node
        fir::read_node *read_node = dynamic_cast<fir::read_node *>(expr);
        if (read_node)
          expr->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
      }
      else if (expr->is_typed(cdk::TYPE_VOID) || expr->is_typed(cdk::TYPE_POINTER))
        throw std::string("Wrong type in argument's of write.");
    }
  }
  
}

//---------------------------------------------------------------------------

void fir::type_checker::do_while_node(fir::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->condition());
    if (read_node)
      node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  }

  node->block()->accept(this, lvl + 4);
  if (node->finally_block()) node->finally_block()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_if_node(fir::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->condition());
    if (read_node)
      node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  }
}

void fir::type_checker::do_if_else_node(fir::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *read_node = dynamic_cast<fir::read_node *>(node->condition());
    if (read_node)
      node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));  // only allow ints
  }
}


void fir::type_checker::do_leave_node(fir::leave_node *const node, int lvl) {
  if (node->cycle_number() < FIR_AST_LEAVE_NODE_DEFAULT)
    throw std::string("Invalid number os cycles to leave");
}

void fir::type_checker::do_restart_node(fir::restart_node *const node, int lvl) {
  if (node->cycle_number() < FIR_AST_RESTART_NODE_DEFAULT)
    throw std::string("Invalid number os cycles to restart");
}

void fir::type_checker::do_return_node(fir::return_node *const node, int lvl) {
  //EMPTY
}

