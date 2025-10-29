#ifndef PASS1_HPP
#define PASS1_HPP

#include "directive.hpp"
#include "error_handler.hpp"
#include "line.hpp"
#include "literal_table.hpp"
#include "opcode_encoder.hpp"
#include "symbol_table.hpp"
#include "warning_handler.hpp"
#include <string>
#include <vector>

namespace sicxe {

class Pass1 {
public:
  explicit Pass1(const OpcodeEncoder &encoder);

  void process(const std::string &filename);
  const SymbolTable &get_symbol_table() const { return symbol_table_; }
  const LiteralTable &get_literal_table() const { return literal_table_; }
  const std::vector<Line> &get_lines() const { return lines_; }
  int get_program_length() const { return program_length_; }
  int get_start_address() const { return start_address_; }

  [[nodiscard]] bool has_errors() const { return error_handler_.has_errors(); }
  [[nodiscard]] const Pass1ErrorHandler &get_error_handler() const {
    return error_handler_;
  }
  void clear_errors() { error_handler_.clear(); }

  [[nodiscard]] bool has_warnings() const {
    return warning_handler_.has_warnings();
  }
  [[nodiscard]] const Pass1WarningHandler &get_warning_handler() const {
    return warning_handler_;
  }
  void clear_warnings() { warning_handler_.clear(); }

private:
  void process_line(const std::string &source_line, int line_num);
  Line parse_line(const std::string &source_line, int line_num);

  void classify_line(Line &line);
  void parse_addressing_mode(Line &line);
  int calculate_size(const Line &line);

  const OpcodeEncoder &encoder_;
  DirectiveRegistry directive_registry_;
  SymbolTable symbol_table_;
  LiteralTable literal_table_;
  std::vector<Line> lines_;
  int location_counter_;
  int start_address_;
  int program_length_;

  Pass1ErrorHandler error_handler_;
  Pass1WarningHandler warning_handler_;
};

} // namespace sicxe

#endif // PASS1_HPP
