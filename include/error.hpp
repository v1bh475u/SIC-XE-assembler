#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

namespace sicxe {

enum class ErrorCode {
  OK = 0,
  // Syntax errors
  SYNTAX_ERROR,
  INVALID_OPERAND,
  INVALID_DIRECTIVE,
  INVALID_OPCODE,
  INVALID_FORMAT,
  MALFORMED_LITERAL,

  // Symbol table errors
  DUPLICATE_SYMBOL,
  UNDEFINED_SYMBOL,

  // Addressing errors
  ADDRESSING_ERROR,
  DISPLACEMENT_OUT_OF_RANGE,
  INVALID_ADDRESSING_MODE,

  // File errors
  FILE_NOT_FOUND,
  FILE_WRITE_ERROR,

  // General errors
  UNKNOWN_ERROR
};

struct ErrorInfo {
  ErrorCode code;
  std::string message;
  int line_number = -1;

  ErrorInfo(ErrorCode c, const std::string &msg, int line = -1)
      : code(c), message(msg), line_number(line) {}

  std::string to_string() const {
    std::string result;
    if (line_number >= 0) {
      result = "Line " + std::to_string(line_number) + ": ";
    }
    result += message;
    return result;
  }
};

} // namespace sicxe

#endif // ERROR_HPP
