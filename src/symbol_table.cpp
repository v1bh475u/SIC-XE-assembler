#include "symbol_table.hpp"

namespace sicxe {

Result<bool, ErrorInfo> SymbolTable::insert(const Symbol &symbol) {
  if (exists(symbol.name)) {
    return ErrorInfo(ErrorCode::DUPLICATE_SYMBOL,
                     "Duplicate symbol: " + symbol.name);
  }
  symbols_.insert({symbol.name, symbol});
  return true;
}

Result<bool, ErrorInfo> SymbolTable::insert(const std::string &name,
                                            int address) {
  return insert(Symbol(name, address));
}

std::optional<Symbol> SymbolTable::lookup(const std::string &name) const {
  auto it = symbols_.find(name);
  if (it != symbols_.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::optional<int> SymbolTable::get_address(const std::string &name) const {
  auto sym = lookup(name);
  if (sym.has_value()) {
    return sym->address;
  }
  return std::nullopt;
}

bool SymbolTable::exists(const std::string &name) const {
  return symbols_.find(name) != symbols_.end();
}

} // namespace sicxe
