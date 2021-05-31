#ifndef __FIR_AST_PROLOGUE_H__
#define __FIR_AST_PROLOGUE_H__

#include <cdk/ast/basic_node.h>
#include "block_node.h"

namespace fir {

  /**
   * Class for describing prologue nodes.
   */
  class prologue_node: public fir::block_node {

  public:
    prologue_node(int lineno, cdk::sequence_node *declarations, cdk::sequence_node *instructions) :
        fir::block_node(lineno, declarations, instructions) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_prologue_node(this, level);
    }

  };

} // fir

#endif
