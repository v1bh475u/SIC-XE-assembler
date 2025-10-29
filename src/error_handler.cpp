#include "error_handler.hpp"
#include <iostream>
#include <sstream>

namespace sicxe {

std::string Error::to_string() const {
  std::ostringstream oss;

  // Format: [Component] Line X: Message (context)
  if (line_number >= 0) {
    oss << "Line " << line_number << ": ";
  }

  oss << message;

  if (!context.empty()) {
    oss << " (" << context << ")";
  }

  return oss.str();
}

void ErrorHandler::add_error(ErrorCode code, const std::string &message,
                             int line_number, const std::string &context) {
  errors_.emplace_back(code, message, line_number, context);
}

void ErrorHandler::add_error(const Error &error) { errors_.push_back(error); }

void ErrorHandler::clear() { errors_.clear(); }

std::string ErrorHandler::format_errors() const {
  if (errors_.empty()) {
    return "";
  }

  std::ostringstream oss;
  oss << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
  oss << component_name_ << " Errors (" << errors_.size() << ")\n";
  oss << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

  for (size_t i = 0; i < errors_.size(); ++i) {
    oss << "[" << (i + 1) << "] " << errors_[i].to_string() << "\n";
  }

  oss << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

  return oss.str();
}

void ErrorHandler::print_errors() const {
  if (!errors_.empty()) {
    std::cerr << format_errors();
  }
}

void Pass1ErrorHandler::add_duplicate_symbol_error(const std::string &symbol,
                                                   int line) {
  add_error(ErrorCode::DUPLICATE_SYMBOL, "Duplicate symbol: " + symbol, line,
            symbol);
}

void Pass1ErrorHandler::add_invalid_opcode_error(const std::string &opcode,
                                                 int line) {
  add_error(ErrorCode::INVALID_OPCODE, "Invalid or unknown opcode: " + opcode,
            line, opcode);
}

void Pass1ErrorHandler::add_invalid_start_address_error(int line) {
  add_error(ErrorCode::SYNTAX_ERROR,
            "Invalid START address - must be a hexadecimal value", line);
}

void Pass1ErrorHandler::add_syntax_error(const std::string &message, int line) {
  add_error(ErrorCode::SYNTAX_ERROR, message, line);
}

void Pass1ErrorHandler::add_invalid_directive_error(
    const std::string &directive, int line) {
  add_error(ErrorCode::INVALID_DIRECTIVE, "Invalid directive: " + directive,
            line, directive);
}

void Pass2ErrorHandler::add_undefined_symbol_error(const std::string &symbol,
                                                   int line) {
  add_error(ErrorCode::UNDEFINED_SYMBOL, "Undefined symbol: " + symbol, line,
            symbol);
}

void Pass2ErrorHandler::add_addressing_error(const std::string &message,
                                             int line) {
  add_error(ErrorCode::ADDRESSING_ERROR, message, line);
}

void Pass2ErrorHandler::add_displacement_error(const std::string &symbol,
                                               int line, int displacement) {
  std::ostringstream oss;
  oss << "Displacement out of range for symbol '" << symbol
      << "' (displacement: " << displacement << ")";
  add_error(ErrorCode::DISPLACEMENT_OUT_OF_RANGE, oss.str(), line, symbol);
}

void Pass2ErrorHandler::add_invalid_operand_error(const std::string &operand,
                                                  int line) {
  add_error(ErrorCode::INVALID_OPERAND, "Invalid operand: " + operand, line,
            operand);
}

void Pass2ErrorHandler::add_format_error(const std::string &message, int line) {
  add_error(ErrorCode::INVALID_FORMAT, message, line);
}

void Pass2ErrorHandler::add_invalid_opcode_error(const std::string &message,
                                                 int line) {
  add_error(ErrorCode::INVALID_OPCODE, message, line);
}

void SymbolTableErrorHandler::add_duplicate_symbol_error(
    const std::string &symbol) {
  add_error(ErrorCode::DUPLICATE_SYMBOL, "Symbol already defined: " + symbol,
            -1, symbol);
}

void SymbolTableErrorHandler::add_undefined_symbol_error(
    const std::string &symbol) {
  add_error(ErrorCode::UNDEFINED_SYMBOL, "Symbol not found: " + symbol, -1,
            symbol);
}

void DirectiveErrorHandler::add_invalid_operand_error(
    const std::string &directive, const std::string &message, int line) {
  add_error(ErrorCode::INVALID_OPERAND, directive + ": " + message, line,
            directive);
}

void DirectiveErrorHandler::add_malformed_literal_error(
    const std::string &literal, int line) {
  add_error(ErrorCode::MALFORMED_LITERAL, "Malformed literal: " + literal, line,
            literal);
}

void DirectiveErrorHandler::add_missing_operand_error(
    const std::string &directive, int line) {
  add_error(ErrorCode::INVALID_OPERAND, directive + " requires an operand",
            line, directive);
}

void FileErrorHandler::add_file_not_found_error(const std::string &filename) {
  add_error(ErrorCode::FILE_NOT_FOUND, "File not found: " + filename, -1,
            filename);
}

void FileErrorHandler::add_file_write_error(const std::string &filename) {
  add_error(ErrorCode::FILE_WRITE_ERROR, "Cannot write to file: " + filename,
            -1, filename);
}

void FileErrorHandler::add_file_read_error(const std::string &filename) {
  add_error(ErrorCode::FILE_NOT_FOUND, "Cannot read file: " + filename, -1,
            filename);
}

} // namespace sicxe
