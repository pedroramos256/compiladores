#ifndef __FIR_AST_RETURN_H__
#define __FIR_AST_RETURN_H__

#include <cdk/ast/basic_node.h>

namespace fir {

  /**
   * Class for describing return nodes.
   */
  class return_node: public cdk::basic_node {

  public:
    return_node(int lineno) :
        cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

}

#endif
