#include "instruction.hpp"

namespace sicxe {

bool OpcodeDefinition::supports_format(int format) const {
  for (int f : supported_formats) {
    if (f == format)
      return true;
  }
  return false;
}

int OpcodeDefinition::default_format() const {
  return supported_formats.empty() ? 3 : supported_formats[0];
}

int Instruction::get_size() const {
  return std::visit([](const auto &fmt) { return fmt.size(); }, data);
}

int Instruction::get_format() const {
  return std::visit(
      [](const auto &fmt) -> int {
        using T = std::decay_t<decltype(fmt)>;
        if constexpr (std::is_same_v<T, Format1>)
          return 1;
        else if constexpr (std::is_same_v<T, Format2>)
          return 2;
        else if constexpr (std::is_same_v<T, Format3>)
          return 3;
        else if constexpr (std::is_same_v<T, Format4>)
          return 4;
        return 0;
      },
      data);
}

} // namespace sicxe
