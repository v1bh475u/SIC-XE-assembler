#ifndef PASS2_HPP
#define PASS2_HPP

#include "directive.hpp"
#include "error_handler.hpp"
#include "instruction.hpp"
#include "line.hpp"
#include "literal_table.hpp"
#include "opcode_encoder.hpp"
#include "symbol_table.hpp"
#include "warning_handler.hpp"
#include <string>
#include <vector>

namespace sicxe {

class Pass2 {
public:
  Pass2(const OpcodeEncoder &encoder, const SymbolTable &symbol_table,
        const LiteralTable &literal_table, int start_address,
        int program_length);

  std::vector<std::string>
  generate_object_code(const std::vector<Line> &lines,
                       const std::string &program_name = "");

  [[nodiscard]] bool has_errors() const { return error_handler_.has_errors(); }
  [[nodiscard]] const Pass2ErrorHandler &get_error_handler() const {
    return error_handler_;
  }
  void clear_errors() { error_handler_.clear(); }

  [[nodiscard]] bool has_warnings() const {
    return warning_handler_.has_warnings();
  }
  [[nodiscard]] const Pass2WarningHandler &get_warning_handler() const {
    return warning_handler_;
  }
  void clear_warnings() { warning_handler_.clear(); }

private:
  std::string encode_instruction(const Line &line);
  std::string encode_format1(const Format1 &fmt);
  std::string encode_format2(const Format2 &fmt);
  std::string encode_format3(const Format3 &fmt);
  std::string encode_format4(const Format4 &fmt);

  int resolve_operand(const std::string &operand, int current_address,
                      const Line &line);
  bool is_operand_relocatable(const Line &line) const;

  std::string generate_header_record(const std::string &prog_name, int start,
                                     int length);
  std::string generate_text_record(int start_addr,
                                   const std::string &object_code);
  std::string generate_modification_record(int address, int length_half_bytes);
  std::string generate_end_record(int first_exec_addr);

  const OpcodeEncoder &encoder_;
  const SymbolTable &symbol_table_;
  const LiteralTable &literal_table_;
  DirectiveRegistry directive_registry_;
  int start_address_;
  int program_length_;

  bool base_register_set_ = false;
  int base_register_value_ = 0;

  Pass2ErrorHandler error_handler_;
  Pass2WarningHandler warning_handler_;
};

} // namespace sicxe

#endif // PASS2_HPP
