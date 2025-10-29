#include "pass2.hpp"
#include "error_handler.hpp"
#include <iomanip>
#include <sstream>

namespace sicxe {

Pass2::Pass2(const OpcodeEncoder &encoder, const SymbolTable &symbol_table,
             int start_address, int program_length)
    : encoder_(encoder), symbol_table_(symbol_table),
      start_address_(start_address), program_length_(program_length) {}

std::vector<std::string>
Pass2::generate_object_code(const std::vector<Line> &lines,
                            const std::string &program_name) {
  std::vector<std::string> object_code;

  // Generate Header record
  std::string prog_name = program_name.empty() ? "PROG" : program_name;

  if (prog_name.length() > 6) {
    std::string original = prog_name;
    prog_name = prog_name.substr(0, 6);
    warning_handler_.add_program_name_truncated_warning(original, prog_name);
  }

  object_code.push_back(
      generate_header_record(prog_name, start_address_, program_length_));

  // Generate Text records and track modification records
  std::string current_text;
  int text_start_addr = -1;
  int first_exec_addr = start_address_;
  std::vector<std::string> modification_records;

  for (const auto &line : lines) {
    if (line.is_comment() || line.is_empty()) {
      continue;
    }

    DirectiveType dir_type = string_to_directive(line.mnemonic);

    // Extract program name from START directive
    if (dir_type == DirectiveType::START) {
      if (line.label.has_value()) {
        prog_name = line.label.value();

        if (prog_name.length() > 6) {
          std::string original = prog_name;
          prog_name = prog_name.substr(0, 6);
          warning_handler_.add_program_name_truncated_warning(original,
                                                              prog_name);
        }

        // Regenerate header with correct name
        object_code[0] =
            generate_header_record(prog_name, start_address_, program_length_);
      }
      continue;
    }

    // Track first executable address from END directive
    if (dir_type == DirectiveType::END) {
      if (line.operand.has_value()) {
        auto addr = symbol_table_.get_address(line.operand.value());
        if (addr.has_value()) {
          first_exec_addr = addr.value();
        }
      }
      continue;
    }

    if (dir_type == DirectiveType::BASE) {
      if (line.operand.has_value()) {
        auto addr = symbol_table_.get_address(line.operand.value());
        if (addr.has_value()) {
          base_register_set_ = true;
          base_register_value_ = addr.value();
        } else {
          error_handler_.add_undefined_symbol_error(line.operand.value(),
                                                    line.line_number);
        }
      }
      continue; // BASE generates no object code
    }

    if (dir_type == DirectiveType::NOBASE) {
      base_register_set_ = false;
      base_register_value_ = 0;
      continue; // NOBASE generates no object code
    }

    // Check if this is a reserved data directive (RESB/RESW)
    // These don't generate object code and should terminate the current text
    // record
    if (dir_type == DirectiveType::RESB || dir_type == DirectiveType::RESW) {
      if (!current_text.empty() && text_start_addr != -1) {
        object_code.push_back(
            generate_text_record(text_start_addr, current_text));
        current_text.clear();
        text_start_addr = -1;
      }
      // Skip reserved data - no object code generated
      continue;
    }

    std::string code = encode_instruction(line);
    if (!code.empty()) {
      bool needs_modification = false;
      int mod_address = 0;
      int mod_length = 0;

      if (is_operand_relocatable(line)) {
        // Format 4: 20-bit address field (5 half-bytes)
        if (line.is_extended && code.length() == 8) {
          needs_modification = true;
          mod_address = line.address + 1; // Address field starts at byte 1
          mod_length = 5;                 // 20 bits = 5 half-bytes
        }
        // Format 3 with SIC-compatible addressing (n=0, i=0): 12-bit address (3
        // half-bytes) but we need to modify only if it's direct addressing
        else if (code.length() == 6 &&
                 line.addressing_mode == AddressingMode::SIC_COMPATIBLE) {
          needs_modification = true;
          mod_address = line.address + 1; // Address field starts at byte 1
          mod_length = 3; // 12 bits = 3 half-bytes (actually we want to modify
                          // the full address)
        }
      }

      if (needs_modification) {
        modification_records.push_back(
            generate_modification_record(mod_address, mod_length));
      }

      // Check if adding this instruction would exceed 30 bytes (60 hex chars)
      if (!current_text.empty() && current_text.length() + code.length() > 60) {
        object_code.push_back(
            generate_text_record(text_start_addr, current_text));
        current_text.clear();
        text_start_addr = -1;
      }

      // Start a new text record if needed
      if (text_start_addr == -1) {
        text_start_addr = line.address;
      }

      current_text += code;
    }
  }

  // Flush any remaining text
  if (!current_text.empty() && text_start_addr != -1) {
    object_code.push_back(generate_text_record(text_start_addr, current_text));
  }

  for (const auto &mod_record : modification_records) {
    object_code.push_back(mod_record);
  }

  object_code.push_back(generate_end_record(first_exec_addr));

  return object_code;
}

std::string Pass2::generate_header_record(const std::string &prog_name,
                                          int start, int length) {
  std::ostringstream oss;

  // Format: H^name^start_address^program_length
  std::string padded_name = prog_name;
  padded_name.resize(6, ' '); // Pad to 6 characters

  oss << "H" << padded_name << std::hex << std::uppercase << std::setfill('0')
      << std::setw(6) << start << std::setw(6) << length;

  return oss.str();
}

std::string Pass2::generate_text_record(int start_addr,
                                        const std::string &object_code) {
  std::ostringstream oss;

  // Format: T^start_address^length^object_code
  int byte_length = object_code.length() / 2; // Convert hex chars to bytes

  oss << "T" << std::hex << std::uppercase << std::setfill('0') << std::setw(6)
      << start_addr << std::setw(2) << byte_length << object_code;

  return oss.str();
}

std::string Pass2::generate_modification_record(int address,
                                                int length_half_bytes) {
  std::ostringstream oss;

  // Format: M^address^length_in_half_bytes
  // For format 4 instructions, the address field starts at byte 1 (after
  // opcode) and is 20 bits (5 hex digits = 5 half-bytes)
  oss << "M" << std::hex << std::uppercase << std::setfill('0') << std::setw(6)
      << address << std::setw(2) << length_half_bytes;

  return oss.str();
}

std::string Pass2::generate_end_record(int first_exec_addr) {
  std::ostringstream oss;

  // Format: E^first_executable_address
  oss << "E" << std::hex << std::uppercase << std::setfill('0') << std::setw(6)
      << first_exec_addr;

  return oss.str();
}

std::string Pass2::encode_instruction(const Line &line) {
  // Handle directives
  if (directive_registry_.is_directive(line.mnemonic)) {
    auto handler = directive_registry_.get_handler(line.mnemonic);
    if (handler) {
      return handler->generate_object_code(line, symbol_table_);
    }
    return "";
  }

  // Handle instructions
  auto opcode_def = encoder_.get_opcode_definition(line.mnemonic);
  if (!opcode_def.has_value()) {
    error_handler_.add_invalid_opcode_error(
        "Unknown instruction: " + line.mnemonic, line.line_number);
    return "";
  }

  // Determine format
  int format = opcode_def->default_format();
  if (line.is_extended) {
    if (!opcode_def->supports_format(4)) {
      error_handler_.add_format_error(
          line.mnemonic + " does not support format 4", line.line_number);
      return "";
    }
    format = 4;
  }

  // Build instruction based on format
  if (format == 1) {
    Format1 fmt;
    fmt.opcode = opcode_def->opcode;
    return encode_format1(fmt);
  } else if (format == 2) {
    Format2 fmt;
    fmt.opcode = opcode_def->opcode;
    fmt.r1 = 0; // TODO: Parse register operands
    fmt.r2 = 0;
    return encode_format2(fmt);
  } else if (format == 3) {
    Format3 fmt;
    fmt.opcode = opcode_def->opcode;
    fmt.flags = AddressingFlags::from_mode(line.addressing_mode,
                                           line.is_indexed, false);

    // Resolve operand to displacement
    if (line.operand.has_value()) {
      int target_address =
          resolve_operand(line.operand.value(), line.address, line);
      int pc = line.address + 3;
      int displacement = target_address - pc;

      // For immediate mode, the value is in the displacement field directly
      if (line.addressing_mode == AddressingMode::IMMEDIATE) {
        // Immediate value goes directly in displacement (12 bits)
        fmt.displacement = target_address & 0xFFF;
        fmt.flags.p = false; // No PC-relative for immediate
        fmt.flags.b = false;
      }
      // Check if PC-relative fits in 12 bits (-2048 to 2047)
      else if (displacement >= -2048 && displacement <= 2047) {
        fmt.flags.p = true;
        fmt.flags.b = false;
        fmt.displacement = displacement & 0xFFF;
      } else {
        bool base_relative_ok = false;
        if (base_register_set_) {
          int base_displacement = target_address - base_register_value_;
          // Base-relative: displacement must be in range [0, 4095] (12 bits
          // unsigned)
          if (base_displacement >= 0 && base_displacement <= 4095) {
            fmt.flags.p = false;
            fmt.flags.b = true;
            fmt.displacement = base_displacement & 0xFFF;
            base_relative_ok = true;
          }
        }

        // If base-relative didn't work, it's an error
        if (!base_relative_ok) {
          error_handler_.add_addressing_error(
              "Address out of range for PC-relative and base-relative "
              "addressing. "
              "Use format 4 (+) or set BASE register.",
              line.line_number);
          // Use direct addressing as fallback (will likely be wrong)
          fmt.flags.p = false;
          fmt.flags.b = false;
          fmt.displacement = target_address & 0xFFF;
        }
      }
    } else {
      fmt.displacement = 0;
    }

    return encode_format3(fmt);
  } else if (format == 4) {
    Format4 fmt;
    fmt.opcode = opcode_def->opcode;
    fmt.flags =
        AddressingFlags::from_mode(line.addressing_mode, line.is_indexed, true);

    if (line.operand.has_value()) {
      fmt.address = resolve_operand(line.operand.value(), line.address, line);
    } else {
      fmt.address = 0;
    }

    return encode_format4(fmt);
  }

  return "";
}

std::string Pass2::encode_format1(const Format1 &fmt) {
  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)
      << static_cast<int>(fmt.opcode);
  return oss.str();
}

std::string Pass2::encode_format2(const Format2 &fmt) {
  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)
      << static_cast<int>(fmt.opcode) << std::setw(1)
      << static_cast<int>(fmt.r1) << std::setw(1) << static_cast<int>(fmt.r2);
  return oss.str();
}

std::string Pass2::encode_format3(const Format3 &fmt) {
  // Build the 3-byte instruction
  // Byte 1: opcode (6 bits) + n (1 bit) + i (1 bit)
  uint8_t byte1 = fmt.opcode;
  if (fmt.flags.n)
    byte1 |= 0x02;
  if (fmt.flags.i)
    byte1 |= 0x01;

  // Byte 2: x (1 bit) + b (1 bit) + p (1 bit) + e (1 bit) + disp[11:8] (4 bits)
  uint8_t byte2 = (fmt.displacement >> 8) & 0x0F;
  if (fmt.flags.x)
    byte2 |= 0x80;
  if (fmt.flags.b)
    byte2 |= 0x40;
  if (fmt.flags.p)
    byte2 |= 0x20;
  if (fmt.flags.e)
    byte2 |= 0x10;

  // Byte 3: disp[7:0]
  uint8_t byte3 = fmt.displacement & 0xFF;

  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)
      << static_cast<int>(byte1) << std::setw(2) << static_cast<int>(byte2)
      << std::setw(2) << static_cast<int>(byte3);

  return oss.str();
}

std::string Pass2::encode_format4(const Format4 &fmt) {
  // Build the 4-byte instruction
  // Similar to format 3 but with 20-bit address instead of 12-bit displacement
  uint8_t byte1 = fmt.opcode;
  if (fmt.flags.n)
    byte1 |= 0x02;
  if (fmt.flags.i)
    byte1 |= 0x01;

  uint8_t byte2 = (fmt.address >> 16) & 0x0F;
  if (fmt.flags.x)
    byte2 |= 0x80;
  if (fmt.flags.b)
    byte2 |= 0x40;
  if (fmt.flags.p)
    byte2 |= 0x20;
  if (fmt.flags.e)
    byte2 |= 0x10;

  uint8_t byte3 = (fmt.address >> 8) & 0xFF;
  uint8_t byte4 = fmt.address & 0xFF;

  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)
      << static_cast<int>(byte1) << std::setw(2) << static_cast<int>(byte2)
      << std::setw(2) << static_cast<int>(byte3) << std::setw(2)
      << static_cast<int>(byte4);

  return oss.str();
}

bool Pass2::is_operand_relocatable(const Line &line) const {
  // No operand means not relocatable
  if (!line.operand.has_value()) {
    return false;
  }

  std::string operand = line.operand.value();

  // Immediate addressing mode: value is absolute (not relocatable)
  if (line.addressing_mode == AddressingMode::IMMEDIATE) {
    return false;
  }

  // Check if operand is a symbol
  auto symbol = symbol_table_.lookup(operand);
  if (symbol.has_value()) {
    // Return the relocatable status of the symbol
    return symbol->is_relocatable;
  }

  // If it's a numeric literal, it's absolute
  return false;
}

int Pass2::resolve_operand(const std::string &operand, int /*current_address*/,
                           const Line &line) {
  // Handle immediate values
  if (line.addressing_mode == AddressingMode::IMMEDIATE) {
    try {
      // Try decimal first
      return std::stoi(operand);
    } catch (...) {
      // Try hex if decimal fails
      try {
        return std::stoi(operand, nullptr, 16);
      } catch (...) {
        error_handler_.add_invalid_operand_error(
            "Invalid immediate value: " + operand, line.line_number);
        return 0; // Return 0 on error
      }
    }
  }

  // Handle symbols
  auto addr = symbol_table_.get_address(operand);
  if (addr.has_value()) {
    return addr.value();
  }

  // Try as a numeric value
  try {
    return std::stoi(operand);
  } catch (...) {
    try {
      return std::stoi(operand, nullptr, 16);
    } catch (...) {
      error_handler_.add_undefined_symbol_error(operand, line.line_number);
      return 0; // Return 0 on error
    }
  }
}

} // namespace sicxe
