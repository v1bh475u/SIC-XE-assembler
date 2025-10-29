#include "warning_handler.hpp"
#include <iostream>
#include <sstream>

namespace sicxe {

std::string Warning::to_string() const {
  std::ostringstream oss;

  // Format: Line X: Message (context)
  if (line_number >= 0) {
    oss << "Line " << line_number << ": ";
  }

  oss << message;

  if (!context.empty()) {
    oss << " (" << context << ")";
  }

  return oss.str();
}

void WarningHandler::add_warning(WarningCode code, const std::string &message,
                                 int line_number, const std::string &context) {
  warnings_.emplace_back(code, message, line_number, context);
}

void WarningHandler::add_warning(const Warning &warning) {
  warnings_.push_back(warning);
}

void WarningHandler::clear() { warnings_.clear(); }

std::string WarningHandler::format_warnings() const {
  if (warnings_.empty()) {
    return "";
  }

  std::ostringstream oss;
  oss << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
  oss << component_name_ << " Warnings (" << warnings_.size() << ")\n";
  oss << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

  for (size_t i = 0; i < warnings_.size(); ++i) {
    oss << "[" << (i + 1) << "] " << warnings_[i].to_string() << "\n";
  }

  oss << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

  return oss.str();
}

void WarningHandler::print_warnings() const {
  if (!warnings_.empty()) {
    std::cerr << format_warnings();
  }
}

void Pass1WarningHandler::add_symbol_truncated_warning(
    const std::string &original, const std::string &truncated, int line) {
  std::ostringstream oss;
  oss << "Symbol '" << original << "' truncated to '" << truncated
      << "' (max 6 characters)";
  add_warning(WarningCode::SYMBOL_TRUNCATED, oss.str(), line, original);
}

void Pass1WarningHandler::add_program_name_truncated_warning(
    const std::string &original, const std::string &truncated, int line) {
  std::ostringstream oss;
  oss << "Program name '" << original << "' truncated to '" << truncated
      << "' (max 6 characters)";
  add_warning(WarningCode::PROGRAM_NAME_TRUNCATED, oss.str(), line, original);
}

void Pass2WarningHandler::add_program_name_truncated_warning(
    const std::string &original, const std::string &truncated) {
  std::ostringstream oss;
  oss << "Program name '" << original << "' truncated to '" << truncated
      << "' (max 6 characters)";
  add_warning(WarningCode::PROGRAM_NAME_TRUNCATED, oss.str(), -1, original);
}

} // namespace sicxe
