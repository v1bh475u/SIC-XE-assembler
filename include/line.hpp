#ifndef LINE_HPP
#define LINE_HPP

#include "addressing_mode.hpp"
#include <optional>
#include <string>

namespace sicxe {

enum class LineType {
  INSTRUCTION, // Machine instruction (LDA, ADD, etc.)
  DIRECTIVE,   // Assembler directive (START, BYTE, WORD, etc.)
  COMMENT,     // Comment line
  EMPTY        // Empty or whitespace-only line
};

struct Line {
  // Source information
  std::optional<std::string> label;
  std::string mnemonic;
  std::optional<std::string> operand;
  int line_number = 0; // Source line number for error reporting

  LineType type = LineType::EMPTY;

  // Address assignment (set during Pass 1)
  int address = 0;

  AddressingMode addressing_mode = AddressingMode::SIMPLE;
  bool is_extended = false; // true if format 4 (e.g., +LDA)
  bool is_indexed = false;  // true if indexed (,X)

  [[nodiscard]] bool is_comment() const { return type == LineType::COMMENT; }
  [[nodiscard]] bool is_directive() const {
    return type == LineType::DIRECTIVE;
  }
  [[nodiscard]] bool is_instruction() const {
    return type == LineType::INSTRUCTION;
  }
  [[nodiscard]] bool is_empty() const { return type == LineType::EMPTY; }

  [[nodiscard]] bool has_label() const {
    return label.has_value() && !label->empty();
  }
  [[nodiscard]] bool has_operand() const {
    return operand.has_value() && !operand->empty();
  }
};

} // namespace sicxe

#endif // LINE_HPP
