#include <string>
#include <cdk/types/types.h>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "targets/symbol.h"
#include "ast/all.h"  // automatically generated
#include "fir_parser.tab.h"


static std::string qualifier_name(int qualifier) {
  if (qualifier == tPRIVATE) return "private";
  else if (qualifier == tPUBLIC) return "public";
  else if (qualifier == tEXTERNAL) return "external";
  else return "unknown qualifier";
}


//---------------------------------------------------------------------------

//nao usado no projeto
void fir::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}

//nao usado no projeto
void fir::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void fir::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}

//nao usado no projeto
void fir::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}

void fir::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

void fir::xml_writer::do_identity_node(fir::identity_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}


//---------------------------------------------------------------------------

void fir::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++){
    cdk::basic_node *n = node->node(i);
    if (n == NULL) break;
    n->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void fir::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

void fir::xml_writer::do_null_node(fir::null_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->argument())
    node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//nao encontro isto no cdk
void fir::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

void fir::xml_writer::do_sizeof_node(fir::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->expression())
    node->expression()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_address_of_node(fir::address_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->lvalue())
    node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}


//---------------------------------------------------------------------------

void fir::xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->left())  
    node->left()->accept(this, lvl + 2);
  if(node->right())
    node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void fir::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void fir::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->lvalue())
    node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  if(node->lvalue())
    node->lvalue()->accept(this, lvl + 2);
  reset_new_symbol();

  if(node->rvalue())
    node->rvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_stack_alloc_node(fir::stack_alloc_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

void fir::xml_writer::do_index_node(fir::index_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("base",lvl + 2);
  if(node->base())
    node->base()->accept(this, lvl + 4);
  closeTag("base",lvl + 2);

  reset_new_symbol();

  openTag("index",lvl + 2);
  if(node->index())
    node->index()->accept(this, lvl + 4);
  closeTag("index",lvl + 2);

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_function_definition_node(fir::function_definition_node * const node, int lvl) {
 
  // ASSERT_SAFE_EXPRESSIONS;

  // nao sei se no nosso enunciado isto e' relevante
  // nao consegui encontrar nada a dizer que nao era proibido definir funcoes dentro do body ou dos argumentos
  /*
  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot define function in body or in args");
    return;
  }
  */
  
  // remember symbol so that args and body know
  //_function = new_symbol();
  //reset_new_symbol();
  
  //_inFunctionBody = true;
  _symtab.push(); // scope of args
  
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() 
    << "' qualifier='" << qualifier_name(node->qualifier()) << "' type='" << cdk::to_string(node->type()) << "'>" << std::endl;

  openTag("arguments", lvl + 2);
  if (node->arguments()) {
    //_inFunctionArgs = true; //FIXME really needed? Nao e' necessario porque o _inFunctionBody ja' e' true
    node->arguments()->accept(this, lvl + 4); //nao sei porque usam +4 em vez de +2
    //_inFunctionArgs = false; //FIXME really needed? 
  }
  closeTag("arguments", lvl + 2);

  reset_new_symbol();

  openTag("return_value",lvl + 2);
  if (node->return_value()) 
    node->return_value()->accept(this, lvl + 4);
  closeTag("return_value", lvl + 2);
  
  reset_new_symbol();

  openTag("prologue",lvl + 2);
  if (node->prologue())
    node->prologue()->accept(this, lvl + 4);
  closeTag("prologue", lvl + 2);
  
  reset_new_symbol();
  
  openTag("body",lvl + 2);
  if (node->body())
    node->body()->accept(this, lvl + 4);
  closeTag("body", lvl + 2);

  reset_new_symbol();

  openTag("epilogue",lvl + 2);
  if (node->epilogue())
    node->epilogue()->accept(this, lvl + 4);
  closeTag("epilogue", lvl + 2);
  
  closeTag(node, lvl);

  _symtab.pop(); // scope of args
  //_inFunctionBody = false;
}

void fir::xml_writer::do_function_declaration_node(fir::function_declaration_node * const node, int lvl) {
  
  // ASSERT_SAFE_EXPRESSIONS;

  // nao sei se no nosso enunciado isto e' relevante
  // nao consegui encontrar nada a dizer que nao era proibido definir funcoes dentro do body ou dos argumentos
  /*
  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot declare function in body or in args");
    return;
  }
  */

  // reset_new_symbol();//nao sei quando e' que isto e' necessario

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "' qualifier='"
      << qualifier_name(node->qualifier()) << "' type='" << cdk::to_string(node->type()) << "'>" << std::endl;

  openTag("arguments", lvl + 2);
  if (node->arguments()) {
    _symtab.push();
    node->arguments()->accept(this, lvl + 4);
    _symtab.pop();
  }
  closeTag("arguments", lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_variable_declaration_node(fir::variable_declaration_node * const node, int lvl) {
  
  // ASSERT_SAFE_EXPRESSIONS;
  // reset_new_symbol();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() 
    << "' qualifier='" << qualifier_name(node->qualifier()) << "' type='" << cdk::to_string(node->type()) << "'>" << std::endl;
  
  openTag("initializer", lvl + 2);
  if (node->initializer())
    node->initializer()->accept(this, lvl + 4);
  closeTag("initializer", lvl + 2);
  
  closeTag(node, lvl);
}

void fir::xml_writer::do_block_node(fir::block_node * const node, int lvl) {
  
  // ASSERT_SAFE_EXPRESSIONS;

  openTag(node,lvl);
  
  openTag("declarations",lvl + 2);
  if (node->declarations())
    node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl + 2);

  reset_new_symbol();

  openTag("instructions",lvl + 2);
  if (node->instructions())
    node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);
  
  closeTag(node, lvl);
}

void fir::xml_writer::do_prologue_node(fir::prologue_node * const node, int lvl) {
  
  // ASSERT_SAFE_EXPRESSIONS;

  openTag(node,lvl);
  
  openTag("declarations",lvl + 2);
  if (node->declarations())
    node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl + 2);

  reset_new_symbol();

  openTag("instructions",lvl + 2);
  if (node->instructions())
    node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);
  
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_evaluation_node(fir::evaluation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void fir::xml_writer::do_function_call_node(fir::function_call_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  // reset_new_symbol();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "'>" << std::endl;

  openTag("arguments",lvl + 2);
  if (node->arguments())
    node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);

  closeTag(node,lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_read_node(fir::read_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void fir::xml_writer::do_write_node(fir::write_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  
  os() << std::string(lvl, ' ') << "<" << node->label() << " newline='" << node->newline() << "'>" << std::endl;

  openTag("arguments",lvl + 2);
  if (node->arguments())
    node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);  

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_while_node(fir::while_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  
  openTag("condition",lvl + 2);
  if (node->condition())
    node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2); 

  reset_new_symbol();

  openTag("block",lvl + 2);
  if (node->block())
    node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2); 

  reset_new_symbol();

  openTag("finally_block",lvl + 2);
  if (node->finally_block())
    node->finally_block()->accept(this, lvl + 4);
  closeTag("finally_block", lvl + 2); 

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_if_node(fir::if_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  
  openTag("condition", lvl + 2);
  if (node->condition())   
    node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  
  reset_new_symbol();

  openTag("block", lvl + 2);
  if (node->block())   
    node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  
  closeTag(node, lvl);
}

void fir::xml_writer::do_if_else_node(fir::if_else_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  if (node->condition())     
    node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  
  reset_new_symbol();

  openTag("thenblock", lvl + 2);
  if (node->thenblock())   
    node->thenblock()->accept(this, lvl + 4);
  closeTag("thenblock", lvl + 2);

  reset_new_symbol();

  openTag("elseblock", lvl + 2);
  if (node->elseblock())     
    node->elseblock()->accept(this, lvl + 4);
  closeTag("elseblock", lvl + 2);
  
  closeTag(node, lvl);
}

void fir::xml_writer::do_leave_node(fir::leave_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << " cycle_number='" << node->cycle_number() << "'>" << std::endl;
  closeTag(node,lvl);
}

void fir::xml_writer::do_restart_node(fir::restart_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << " cycle_number='" << node->cycle_number() << "'>" << std::endl;
  closeTag(node,lvl);
}

void fir::xml_writer::do_return_node(fir::return_node * const node, int lvl) {
  openTag(node,lvl);
  closeTag(node,lvl);
}

