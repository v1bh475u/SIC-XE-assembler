#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "error.hpp"
#include "result.hpp"
#include "symbol.hpp"
#include <optional>
#include <string>
#include <unordered_map>

namespace sicxe {

class SymbolTable {
public:
  Result<bool, ErrorInfo> insert(const Symbol &symbol);
  Result<bool, ErrorInfo> insert(const std::string &name, int address);

  std::optional<Symbol> lookup(const std::string &name) const;
  std::optional<int> get_address(const std::string &name) const;

  bool exists(const std::string &name) const;

  const std::unordered_map<std::string, Symbol> &get_all() const {
    return symbols_;
  }

private:
  std::unordered_map<std::string, Symbol> symbols_;
};

} // namespace sicxe

#endif // SYMBOL_TABLE_HPP
