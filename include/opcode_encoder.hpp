#ifndef OPCODE_ENCODER_HPP
#define OPCODE_ENCODER_HPP

#include "instruction.hpp"
#include <optional>
#include <string>
#include <unordered_map>

namespace sicxe {

class OpcodeEncoder {
public:
  OpcodeEncoder();

  std::optional<OpcodeDefinition>
  get_opcode_definition(const std::string &mnemonic) const;
  bool is_instruction(const std::string &mnemonic) const;

  // Check if a mnemonic supports a specific format
  bool supports_format(const std::string &mnemonic, int format) const;

private:
  void initialize_opcodes();
  void add_opcode(const std::string &mnemonic, uint8_t opcode,
                  std::vector<int> formats);

  std::unordered_map<std::string, OpcodeDefinition> opcodes_;
};

} // namespace sicxe

#endif // OPCODE_ENCODER_HPP
