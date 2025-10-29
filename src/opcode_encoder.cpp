#include "opcode_encoder.hpp"

namespace sicxe {

OpcodeEncoder::OpcodeEncoder() { initialize_opcodes(); }

void OpcodeEncoder::add_opcode(const std::string &mnemonic, uint8_t opcode,
                               std::vector<int> formats) {
  opcodes_[mnemonic] = {mnemonic, opcode, formats};
}

void OpcodeEncoder::initialize_opcodes() {
  // Format 3/4 instructions (most common)
  add_opcode("ADD", 0x18, {3, 4});
  add_opcode("ADDF", 0x58, {3, 4});
  add_opcode("AND", 0x40, {3, 4});
  add_opcode("COMP", 0x28, {3, 4});
  add_opcode("COMPF", 0x88, {3, 4});
  add_opcode("DIV", 0x24, {3, 4});
  add_opcode("DIVF", 0x64, {3, 4});
  add_opcode("J", 0x3C, {3, 4});
  add_opcode("JEQ", 0x30, {3, 4});
  add_opcode("JGT", 0x34, {3, 4});
  add_opcode("JLT", 0x38, {3, 4});
  add_opcode("JSUB", 0x48, {3, 4});
  add_opcode("LDA", 0x00, {3, 4});
  add_opcode("LDB", 0x68, {3, 4});
  add_opcode("LDCH", 0x50, {3, 4});
  add_opcode("LDF", 0x70, {3, 4});
  add_opcode("LDL", 0x08, {3, 4});
  add_opcode("LDS", 0x6C, {3, 4});
  add_opcode("LDT", 0x74, {3, 4});
  add_opcode("LDX", 0x04, {3, 4});
  add_opcode("LPS", 0xD0, {3, 4});
  add_opcode("MUL", 0x20, {3, 4});
  add_opcode("MULF", 0x60, {3, 4});
  add_opcode("OR", 0x44, {3, 4});
  add_opcode("RD", 0xD8, {3, 4});
  add_opcode("RSUB", 0x4C, {3, 4});
  add_opcode("SSK", 0xEC, {3, 4});
  add_opcode("STA", 0x0C, {3, 4});
  add_opcode("STB", 0x78, {3, 4});
  add_opcode("STCH", 0x54, {3, 4});
  add_opcode("STF", 0x80, {3, 4});
  add_opcode("STI", 0xD4, {3, 4});
  add_opcode("STL", 0x14, {3, 4});
  add_opcode("STS", 0x7C, {3, 4});
  add_opcode("STSW", 0xE8, {3, 4});
  add_opcode("STT", 0x84, {3, 4});
  add_opcode("STX", 0x10, {3, 4});
  add_opcode("SUB", 0x1C, {3, 4});
  add_opcode("SUBF", 0x5C, {3, 4});
  add_opcode("TD", 0xE0, {3, 4});
  add_opcode("TIX", 0x2C, {3, 4});
  add_opcode("WD", 0xDC, {3, 4});

  // Format 2 instructions (register-to-register)
  add_opcode("ADDR", 0x90, {2});
  add_opcode("CLEAR", 0xB4, {2});
  add_opcode("COMPR", 0xA0, {2});
  add_opcode("DIVR", 0x9C, {2});
  add_opcode("MULR", 0x98, {2});
  add_opcode("RMO", 0xAC, {2});
  add_opcode("SHIFTL", 0xA4, {2});
  add_opcode("SHIFTR", 0xA8, {2});
  add_opcode("SUBR", 0x94, {2});
  add_opcode("SVC", 0xB0, {2});
  add_opcode("TIXR", 0xB8, {2});

  // Format 1 instructions
  add_opcode("FIX", 0xC4, {1});
  add_opcode("FLOAT", 0xC0, {1});
  add_opcode("HIO", 0xF4, {1});
  add_opcode("NORM", 0xC8, {1});
  add_opcode("SIO", 0xF0, {1});
  add_opcode("TIO", 0xF8, {1});
}

std::optional<OpcodeDefinition>
OpcodeEncoder::get_opcode_definition(const std::string &mnemonic) const {
  auto it = opcodes_.find(mnemonic);
  if (it != opcodes_.end()) {
    return it->second;
  }
  return std::nullopt;
}

bool OpcodeEncoder::is_instruction(const std::string &mnemonic) const {
  return opcodes_.find(mnemonic) != opcodes_.end();
}

bool OpcodeEncoder::supports_format(const std::string &mnemonic,
                                    int format) const {
  auto it = opcodes_.find(mnemonic);
  if (it != opcodes_.end()) {
    return it->second.supports_format(format);
  }
  return false;
}

} // namespace sicxe
