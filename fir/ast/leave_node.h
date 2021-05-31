#ifndef __FIR_AST_LEAVE_H__
#define __FIR_AST_LEAVE_H__

#include <cdk/ast/basic_node.h>

#define FIR_AST_LEAVE_NODE_DEFAULT 1

namespace fir {

  /**
   * Class for describing leave nodes.
   */
  class leave_node: public cdk::basic_node {
    int _cycle_number = FIR_AST_LEAVE_NODE_DEFAULT;

  public:
    leave_node(int lineno, int cycle_number) :
        cdk::basic_node(lineno), _cycle_number(cycle_number) {
    }

    leave_node(int lineno) :
        cdk::basic_node(lineno){
    }

  public:
    int cycle_number() {
      return _cycle_number;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_leave_node(this, level);
    }

  };

}

#endif
