#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sicxe {

struct Literal {
  std::string name;           // Full literal syntax: =C'EOF' or =X'05' or =*
  std::vector<uint8_t> value; // Actual byte values
  std::optional<int> address; // Address assigned during literal pool dump
  int length;                 // Size in bytes
  bool allow_dedup;           // Whether this literal can be deduplicated
  std::optional<int>
      reference_address; // For =* literals, the address where it's used

  Literal(std::string n, std::vector<uint8_t> val, bool dedup = true)
      : name(std::move(n)), value(std::move(val)), address(std::nullopt),
        length(static_cast<int>(value.size())), allow_dedup(dedup),
        reference_address(std::nullopt) {}

  Literal(std::string n, std::vector<uint8_t> val, int addr, bool dedup = true)
      : name(std::move(n)), value(std::move(val)), address(addr),
        length(static_cast<int>(value.size())), allow_dedup(dedup),
        reference_address(std::nullopt) {}

  bool has_address() const { return address.has_value(); }
};

} // namespace sicxe

#endif // LITERAL_HPP
