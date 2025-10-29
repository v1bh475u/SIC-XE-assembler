#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include "error.hpp"
#include <string>
#include <vector>

namespace sicxe {

struct Error {
  ErrorCode code;
  std::string message;
  int line_number;
  std::string context;

  Error(ErrorCode c, const std::string &msg, int line = -1,
        const std::string &ctx = "")
      : code(c), message(msg), line_number(line), context(ctx) {}

  std::string to_string() const;
};

class ErrorHandler {
public:
  ErrorHandler(const std::string &component_name)
      : component_name_(component_name) {}

  virtual ~ErrorHandler() = default;

  void add_error(ErrorCode code, const std::string &message,
                 int line_number = -1, const std::string &context = "");

  void add_error(const Error &error);

  [[nodiscard]] bool has_errors() const { return !errors_.empty(); }

  [[nodiscard]] size_t error_count() const { return errors_.size(); }

  [[nodiscard]] const std::vector<Error> &get_errors() const { return errors_; }

  void clear();

  [[nodiscard]] std::string format_errors() const;

  void print_errors() const;

protected:
  std::string component_name_;
  std::vector<Error> errors_;
};

class Pass1ErrorHandler : public ErrorHandler {
public:
  Pass1ErrorHandler() : ErrorHandler("Pass 1") {}

  void add_duplicate_symbol_error(const std::string &symbol, int line);
  void add_invalid_opcode_error(const std::string &opcode, int line);
  void add_invalid_start_address_error(int line);
  void add_syntax_error(const std::string &message, int line);
  void add_invalid_directive_error(const std::string &directive, int line);
};

class Pass2ErrorHandler : public ErrorHandler {
public:
  Pass2ErrorHandler() : ErrorHandler("Pass 2") {}

  void add_undefined_symbol_error(const std::string &symbol, int line);
  void add_addressing_error(const std::string &message, int line);
  void add_displacement_error(const std::string &symbol, int line,
                              int displacement);
  void add_invalid_operand_error(const std::string &operand, int line);
  void add_format_error(const std::string &message, int line);
  void add_invalid_opcode_error(const std::string &message, int line);
};

class SymbolTableErrorHandler : public ErrorHandler {
public:
  SymbolTableErrorHandler() : ErrorHandler("Symbol Table") {}

  void add_duplicate_symbol_error(const std::string &symbol);
  void add_undefined_symbol_error(const std::string &symbol);
};

class DirectiveErrorHandler : public ErrorHandler {
public:
  DirectiveErrorHandler() : ErrorHandler("Directive Processing") {}

  void add_invalid_operand_error(const std::string &directive,
                                 const std::string &message, int line);
  void add_malformed_literal_error(const std::string &literal, int line);
  void add_missing_operand_error(const std::string &directive, int line);
};

class FileErrorHandler : public ErrorHandler {
public:
  FileErrorHandler() : ErrorHandler("File I/O") {}

  void add_file_not_found_error(const std::string &filename);
  void add_file_write_error(const std::string &filename);
  void add_file_read_error(const std::string &filename);
};

} // namespace sicxe

#endif // ERROR_HANDLER_HPP
