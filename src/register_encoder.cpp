#include "register_encoder.hpp"
#include <algorithm>

namespace sicxe {

RegisterEncoder::RegisterEncoder() { initialize_registers(); }

void RegisterEncoder::initialize_registers() {
  // SIC/XE registers with their numeric codes
  registers_["A"] = 0; // Accumulator
  registers_["X"] = 1; // Index register
  registers_["L"] = 2; // Linkage register
  registers_["B"] = 3; // Base register
  registers_["S"] = 4; // General purpose register
  registers_["T"] = 5; // General purpose register
  registers_["F"] = 6; // Floating point accumulator

  // Program counter and status word (some assemblers support these)
  registers_["PC"] = 8;
  registers_["SW"] = 9;
}

std::optional<uint8_t>
RegisterEncoder::get_register_number(const std::string &name) const {
  // Convert to uppercase for case-insensitive comparison
  std::string upper_name = name;
  std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(),
                 ::toupper);

  auto it = registers_.find(upper_name);
  if (it != registers_.end()) {
    return it->second;
  }
  return std::nullopt;
}

bool RegisterEncoder::is_register(const std::string &name) const {
  return get_register_number(name).has_value();
}

} // namespace sicxe
