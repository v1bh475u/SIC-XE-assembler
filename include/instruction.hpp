#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "addressing_mode.hpp"
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace sicxe {

// Format 1: 1 byte (opcode only)
struct Format1 {
  uint8_t opcode;

  int size() const { return 1; }
};

// Format 2: 2 bytes (opcode + registers)
struct Format2 {
  uint8_t opcode;
  uint8_t r1; // First register (4 bits)
  uint8_t r2; // Second register (4 bits)

  int size() const { return 2; }
};

// Format 3: 3 bytes (opcode + nixbpe + displacement)
struct Format3 {
  uint8_t opcode;
  AddressingFlags flags;
  int16_t displacement; // 12-bit displacement (signed)

  int size() const { return 3; }
};

// Format 4: 4 bytes (opcode + nixbpe + address)
struct Format4 {
  uint8_t opcode;
  AddressingFlags flags;
  int32_t address; // 20-bit address

  int size() const { return 4; }
};

// Variant to hold any instruction format
using InstructionData = std::variant<Format1, Format2, Format3, Format4>;

// Opcode definition - supports multiple formats
struct OpcodeDefinition {
  std::string mnemonic;
  uint8_t opcode;
  std::vector<int> supported_formats; // e.g., {3, 4} for most instructions

  bool supports_format(int format) const;
  int default_format() const; // Returns the preferred/default format
};

// A parsed/encoded instruction
struct Instruction {
  std::string mnemonic;
  InstructionData data;

  int get_size() const;
  int get_format() const;
};

} // namespace sicxe

#endif // INSTRUCTION_HPP
