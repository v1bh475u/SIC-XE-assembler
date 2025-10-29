#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sicxe {

struct Literal {
  std::string name;           // Full literal syntax: =C'EOF' or =X'05'
  std::vector<uint8_t> value; // Actual byte values
  std::optional<int> address; // Address assigned during literal pool dump
  int length;                 // Size in bytes

  Literal(std::string n, std::vector<uint8_t> val)
      : name(std::move(n)), value(std::move(val)), address(std::nullopt),
        length(static_cast<int>(value.size())) {}

  Literal(std::string n, std::vector<uint8_t> val, int addr)
      : name(std::move(n)), value(std::move(val)), address(addr),
        length(static_cast<int>(value.size())) {}

  bool has_address() const { return address.has_value(); }
};

} // namespace sicxe

#endif // LITERAL_HPP
