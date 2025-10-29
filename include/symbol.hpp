#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <optional>
#include <string>

namespace sicxe {

struct Symbol {
  std::string name;
  int address;

  // Future extensibility
  bool is_relocatable = true; // For relocatable symbols
  bool is_external = false;   // For EXTREF
  bool is_defined = true;     // For EXTDEF
  std::optional<int> length;  // For EQU with length

  Symbol(std::string n, int addr) : name(std::move(n)), address(addr) {}

  Symbol(std::string n, int addr, bool relocatable, bool external = false)
      : name(std::move(n)), address(addr), is_relocatable(relocatable),
        is_external(external) {}
};

} // namespace sicxe

#endif // SYMBOL_HPP
