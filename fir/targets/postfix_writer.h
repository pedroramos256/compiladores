#ifndef __FIR_TARGETS_POSTFIX_WRITER_H__
#define __FIR_TARGETS_POSTFIX_WRITER_H__

#include "targets/basic_ast_visitor.h"

#include <set>
#include <stack>
#include <sstream>
#include <cdk/emitters/basic_postfix_emitter.h>

namespace fir {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<fir::symbol> &_symtab;

    std::set<std::string> _functions_to_declare;
    std::set<std::string> _variables_to_declare;

    // semantic analysis
    bool _inFunctionBody, _inFunctionArgs;
    std::shared_ptr<fir::symbol> _function; // for keeping track of the current function and its arguments
    int _offset; // current framepointer offset (0 means no vars defined)
    std::stack<int> _whileCond, _whileEnd; // for leave/restart
    bool _inFinallyBlock;

    std::string _currentBodyRetLabel; // where to jump when a return occurs of an exclusive section ends

    cdk::basic_postfix_emitter &_pf;
    int _lbl;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<fir::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _inFunctionBody(false), _inFunctionArgs(false),
        _function(nullptr), _offset(0), _inFinallyBlock(false), _currentBodyRetLabel(""), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  protected:
    void do_ScalarLogicalExpression(cdk::binary_operation_node *const node, int lvl);
    void do_GeneralLogicalExpression(cdk::binary_operation_node *const node, int lvl);
  
  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

    void error(int lineno, std::string s) {
      std::cerr << "error: " << lineno << ": " << s << std::endl;
      exit(1);
    }

    void error(int lineno, std::string id, std::string s) {
      std::cerr << "error: " << lineno << ": " << id << " " << s << std::endl;
      exit(1);
    }

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // fir

#endif
