#ifndef CONTROL_SECTION_HPP
#define CONTROL_SECTION_HPP

#include "line.hpp"
#include "literal_table.hpp"
#include "symbol_table.hpp"
#include <string>
#include <vector>

namespace sicxe {

struct ControlSection {
  std::string name;
  int start_address;
  int length;
  std::vector<std::string> extdef_symbols; // Symbols defined for external use
  std::vector<std::string> extref_symbols; // Symbols referenced from external
  std::vector<Line> lines;                 // Lines belonging to this section
  SymbolTable symbol_table;                // Symbol table for this section
  LiteralTable literal_table;              // Literal table for this section

  ControlSection(const std::string &n, int addr)
      : name(n), start_address(addr), length(0) {}
};

} // namespace sicxe

#endif // CONTROL_SECTION_HPP
