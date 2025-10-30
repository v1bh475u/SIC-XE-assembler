#ifndef REGISTER_ENCODER_HPP
#define REGISTER_ENCODER_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

namespace sicxe {

class RegisterEncoder {
public:
  RegisterEncoder();

  // Get register number (0-15)
  std::optional<uint8_t> get_register_number(const std::string &name) const;

  // Check if string is a valid register name
  bool is_register(const std::string &name) const;

private:
  void initialize_registers();

  std::unordered_map<std::string, uint8_t> registers_;
};

} // namespace sicxe

#endif // REGISTER_ENCODER_HPP
