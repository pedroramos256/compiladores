#ifndef __FIR_TARGETS_SYMBOL_H__
#define __FIR_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <vector>
#include <cdk/types/basic_type.h>

namespace fir {

  class symbol {
    
    std::shared_ptr<cdk::basic_type> _type;
    int _qualifier; // qualifiers: '?': external, '*': public, default: private
    std::string _name;

    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;
    int _value;

    bool _initialized; // initialized?
    int _offset = 0; // 0 (zero) means global variable/function
    bool _function; // false for variables
    int _return_value_offset = 0; // 0 means no return value (for void functions)

    std::string _currentBodyRetLabel; // where to jump when a return occurs of an exclusive section ends


  public:
    symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool initialized,
            bool function) :
        _type(type), _qualifier(qualifier), _name(name), _value(0), _initialized(initialized),
        _function(function), _currentBodyRetLabel("") {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }
    int value() const {
      return _value;
    }
    int qualifier() const {
      return _qualifier;
    }
    
    int value(int v) {
      return _value = v;
    }

    bool isInitialized() const {
      return _initialized;
    }

    int offset() const {
      return _offset;
    }

    int return_value_offset() const {
      return _return_value_offset;
    }

    bool global() const {
      return _offset == 0;
    }

    void set_offset(int offset) {
      _offset = offset;
    }

    void set_return_value_offset(int offset) {
      _return_value_offset = offset;
    }

    bool isFunction() const {
      return _function;
    }

    bool isVariable() const {
      return !_function;
    }

    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }

    std::vector<std::shared_ptr<cdk::basic_type>> &argument_types() {
      return _argument_types;
    }

    bool is_argument_types_equal(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      if (_argument_types.size() != types.size()) return false;

      for (size_t ax = 0; ax < _argument_types.size(); ax++)
        if (!this->argument_is_typed(ax, types[ax]->name())) return false;

      // else
      return true;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }
    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const {
      return _argument_types.size();
    }
  
  };

  inline auto make_symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name,
                          bool initialized, bool function) {
    return std::make_shared<symbol>(qualifier, type, name, initialized, function);
  }

} // fir

#endif
