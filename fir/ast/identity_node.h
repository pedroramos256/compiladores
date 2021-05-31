#ifndef __FIR_AST_IDENTITY_NODE_H__
#define __FIR_AST_IDENTITY_NODE_H__

#include <cdk/ast/unary_operation_node.h>

namespace fir {

  /**
   * Class for describing null concept node.
   */
  class identity_node: public cdk::unary_operation_node {
  public:
    identity_node(int lineno, expression_node *argument) :
        cdk::unary_operation_node(lineno, argument) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // fir

#endif
