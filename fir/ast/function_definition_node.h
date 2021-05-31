#ifndef __FIR_AST_FUNCTION_DEFINITION_NODE_H__
#define __FIR_AST_FUNCTION_DEFINITION_NODE_H__

#include <cdk/ast/typed_node.h>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/types/basic_type.h>
#include <cdk/types/typename_type.h>
#include <cdk/ast/integer_node.h>
#include "block_node.h"
#include "prologue_node.h"
#include "null_node.h"

#define FIR_AST_FUNCTION_DEFINITION_NODE_DEFAULT_INTEGER_RETURN(lineno) new cdk::integer_node(lineno, 0)
#define FIR_AST_FUNCTION_DEFINITION_NODE_DEFAULT_POINTER_RETURN(lineno) new fir::null_node(lineno)

namespace fir {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node: public cdk::typed_node {
    int _qualifier; 
    std::string _identifier;
    cdk::sequence_node *_arguments;
    cdk::expression_node *_return_value;
    fir::prologue_node *_prologue;
    fir::block_node *_body;
    fir::block_node *_epilogue;

  public: 
    /**
     * Constructor for a function definition with default return value specified.
     */
    inline function_definition_node(int lineno, std::shared_ptr<cdk::basic_type> funType, int qualifier, 
        const std::string &identifier, cdk::sequence_node *arguments, cdk::expression_node *return_value, 
        fir::prologue_node *prologue, fir::block_node *body, fir::block_node *epilogue) :
          cdk::typed_node(lineno), _qualifier(qualifier),_identifier(identifier), 
          _arguments(arguments), _return_value(return_value),
          _prologue(prologue), _body(body), _epilogue(epilogue) {          
            type(funType);
    }

    /**
     * Constructor for a function definition without default return value sepecified.
     */
    inline function_definition_node(int lineno, std::shared_ptr<cdk::basic_type> funType, int qualifier, 
        const std::string &identifier, cdk::sequence_node *arguments,
        fir::prologue_node *prologue, fir::block_node *body, fir::block_node *epilogue) :
          cdk::typed_node(lineno), _qualifier(qualifier),_identifier(identifier), 
          _arguments(arguments),_return_value(nullptr), _prologue(prologue), _body(body), _epilogue(epilogue) {
            type(funType);
    }

  public:
    inline int qualifier() {
      return _qualifier;
    }
    const std::string& identifier() const {
      return _identifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }
    cdk::typed_node* argument(size_t ix) {
      return dynamic_cast<cdk::typed_node*>(_arguments->node(ix));
    }
    inline cdk::expression_node *return_value() {
      return _return_value;
    }
    inline fir::prologue_node* prologue() {
      return _prologue;
    }
    inline fir::block_node* body() {
      return _body;
    }
    inline fir::block_node* epilogue() {
      return _epilogue;
    }

    void set_default_return_value_integer() {
      _return_value = FIR_AST_FUNCTION_DEFINITION_NODE_DEFAULT_INTEGER_RETURN(lineno());
    }

    void set_default_return_value_pointer() {
      _return_value = FIR_AST_FUNCTION_DEFINITION_NODE_DEFAULT_POINTER_RETURN(lineno());
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // fir

#endif
