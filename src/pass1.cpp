#include "pass1.hpp"
#include "directive.hpp"
#include "error_handler.hpp"
#include "literal_parser.hpp"
#include <fstream>
#include <sstream>

namespace sicxe {

Pass1::Pass1(const OpcodeEncoder &encoder)
    : encoder_(encoder), location_counter_(0), start_address_(0),
      program_length_(0), current_section_index_(0) {}

void Pass1::process(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    FileErrorHandler file_error_handler;
    file_error_handler.add_file_not_found_error(filename);
    // Copy file errors to pass1 error handler
    for (const auto &error : file_error_handler.get_errors()) {
      error_handler_.add_error(error);
    }
    return;
  }

  std::string source_line;
  int line_num = 0;

  while (std::getline(file, source_line)) {
    line_num++;
    if (!source_line.empty()) {
      process_line(source_line, line_num);
    }
  }

  if (!control_sections_.empty() && !lines_.empty()) {
    finalize_current_section();
  }

  program_length_ = location_counter_ - start_address_;
}

void Pass1::process_line(const std::string &source_line, int line_num) {
  Line line = parse_line(source_line, line_num);

  // Skip comments and empty lines
  if (line.is_comment() || line.is_empty()) {
    lines_.push_back(line);
    return;
  }

  // Set address for this line
  line.address = location_counter_;

  // Handle START directive specially - sets starting address
  if (line.type == LineType::DIRECTIVE) {
    DirectiveType dir_type = string_to_directive(line.mnemonic);

    if (dir_type == DirectiveType::START) {
      std::string section_name =
          line.has_label() ? line.label.value() : "DEFAULT";
      int start_addr = 0;

      if (line.has_operand()) {
        std::string operand = line.operand.value();
        try {
          if (operand.length() >= 3 && operand[0] == 'X' &&
              operand[1] == '\'' && operand.back() == '\'') {
            std::string hex_str = operand.substr(2, operand.length() - 3);
            start_addr = std::stoi(hex_str, nullptr, 16);
          } else {
            start_addr = std::stoi(operand, nullptr, 10);
          }
        } catch (...) {
          error_handler_.add_invalid_start_address_error(line_num);
        }
      }

      start_new_section(section_name, start_addr);
      program_name_ = section_name;
      line.address = start_addr;
      lines_.push_back(line);
      return;
    }

    if (dir_type == DirectiveType::CSECT) {
      // Finalize the previous section if it exists and has content
      if (!control_sections_.empty() && !lines_.empty()) {
        finalize_current_section();
      }

      std::string section_name =
          line.has_label() ? line.label.value() : "CSECT";
      start_new_section(section_name, 0);
      program_name_ = section_name;
      line.address = 0;
      lines_.push_back(line);
      return;
    }

    if (dir_type == DirectiveType::EXTDEF) {
      if (!line.has_operand()) {
        error_handler_.add_error(ErrorCode::SYNTAX_ERROR,
                                 "EXTDEF directive requires operands",
                                 line_num);
        lines_.push_back(line);
        return;
      }

      // Parse comma-separated symbols
      std::string operands = line.operand.value();
      std::stringstream ss(operands);
      std::string symbol;
      while (std::getline(ss, symbol, ',')) {
        // Trim whitespace
        symbol.erase(0, symbol.find_first_not_of(" \t"));
        symbol.erase(symbol.find_last_not_of(" \t") + 1);
        if (!symbol.empty()) {
          extdef_symbols_.push_back(symbol);
        }
      }
      lines_.push_back(line);
      return;
    }

    if (dir_type == DirectiveType::EXTREF) {
      if (!line.has_operand()) {
        error_handler_.add_error(ErrorCode::SYNTAX_ERROR,
                                 "EXTREF directive requires operands",
                                 line_num);
        lines_.push_back(line);
        return;
      }

      std::string operands = line.operand.value();
      std::stringstream ss(operands);
      std::string symbol;
      while (std::getline(ss, symbol, ',')) {
        // Trim whitespace
        symbol.erase(0, symbol.find_first_not_of(" \t"));
        symbol.erase(symbol.find_last_not_of(" \t") + 1);
        if (!symbol.empty()) {
          extref_symbols_.push_back(symbol);
          // Add EXTREF symbol to symbol table as external
          Symbol sym(symbol, 0, false, true);
          auto result = symbol_table_.insert(sym);
          if (result.is_err()) {
            error_handler_.add_duplicate_symbol_error(symbol, line_num);
          }
        }
      }
      lines_.push_back(line);
      return;
    }

    if (dir_type == DirectiveType::EQU) {
      if (!line.has_label()) {
        error_handler_.add_error(ErrorCode::SYNTAX_ERROR,
                                 "EQU directive requires a label", line_num);
        lines_.push_back(line);
        return;
      }

      if (!line.has_operand()) {
        error_handler_.add_error(ErrorCode::SYNTAX_ERROR,
                                 "EQU directive requires an operand", line_num);
        lines_.push_back(line);
        return;
      }

      std::string label = line.label.value();
      std::string operand = line.operand.value();
      int value = 0;
      bool valid = false;

      if (operand == "*") {
        value = location_counter_;
        valid = true;
      } else if (operand.length() >= 3 && operand[0] == 'X' &&
                 operand[1] == '\'' && operand.back() == '\'') {
        std::string hex_str = operand.substr(2, operand.length() - 3);
        try {
          value = std::stoi(hex_str, nullptr, 16);
          valid = true;
        } catch (...) {
          error_handler_.add_error(ErrorCode::INVALID_OPERAND,
                                   "EQU operand has invalid hexadecimal format",
                                   line_num);
        }
      } else {
        try {
          value = std::stoi(operand, nullptr, 10);
          valid = true;
        } catch (...) {
          auto symbol = symbol_table_.lookup(operand);
          if (symbol.has_value()) {
            value = symbol->address;
            valid = true;
          } else {
            error_handler_.add_error(
                ErrorCode::UNDEFINED_SYMBOL,
                "EQU operand '" + operand +
                    "' is not a valid value or defined symbol",
                line_num);
          }
        }
      }

      if (valid) {
        if (label.length() > 6) {
          std::string truncated = label.substr(0, 6);
          warning_handler_.add_symbol_truncated_warning(label, truncated,
                                                        line_num);
          label = truncated;
          line.label = label;
        }

        Symbol sym(label, value, false);
        auto result = symbol_table_.insert(sym);
        if (result.is_err()) {
          error_handler_.add_duplicate_symbol_error(label, line_num);
        }
      }

      lines_.push_back(line);
      return;
    }

    if (dir_type == DirectiveType::ORG) {
      if (line.has_label()) {
        std::string label = line.label.value();

        if (label.length() > 6) {
          std::string truncated = label.substr(0, 6);
          warning_handler_.add_symbol_truncated_warning(label, truncated,
                                                        line_num);
          label = truncated;
          line.label = label;
        }

        Symbol sym(label, location_counter_, true);
        auto result = symbol_table_.insert(sym);
        if (result.is_err()) {
          error_handler_.add_duplicate_symbol_error(label, line_num);
        }
      }

      if (line.has_operand()) {
        // ORG with operand: can be a decimal value, hex in X'...' format, or a
        // symbol
        std::string operand = line.operand.value();
        int new_address = -1;
        bool valid = false;

        // Check if operand is in X'...' format (hexadecimal)
        if (operand.length() >= 3 && operand[0] == 'X' && operand[1] == '\'' &&
            operand.back() == '\'') {
          // Extract hex value from X'...'
          std::string hex_str = operand.substr(2, operand.length() - 3);
          try {
            new_address = std::stoi(hex_str, nullptr, 16);
            valid = true;
          } catch (...) {
            error_handler_.add_error(
                ErrorCode::INVALID_OPERAND,
                "ORG operand has invalid hexadecimal format", line_num);
          }
        } else {
          // Try to parse as decimal value first
          try {
            new_address = std::stoi(operand, nullptr, 10);
            valid = true;
          } catch (...) {
            // Not a decimal value, try as a symbol
            auto symbol = symbol_table_.lookup(operand);
            if (symbol.has_value()) {
              new_address = symbol->address;
              valid = true;
            } else {
              error_handler_.add_error(
                  ErrorCode::UNDEFINED_SYMBOL,
                  "ORG operand '" + operand +
                      "' is not a valid address or defined symbol",
                  line_num);
            }
          }
        }

        if (valid) {
          saved_location_counter_ = location_counter_;
          location_counter_ = new_address;
        }
      } else {
        // ORG without operand: restore saved location counter
        if (saved_location_counter_.has_value()) {
          location_counter_ = saved_location_counter_.value();
          saved_location_counter_ = std::nullopt;
        } else {
          error_handler_.add_error(
              ErrorCode::SYNTAX_ERROR,
              "ORG without operand called but no saved location counter exists",
              line_num);
        }
      }
      lines_.push_back(line);
      return;
    }

    if (dir_type == DirectiveType::LTORG) {
      auto unaddressed_names = literal_table_.get_unaddressed_literal_names();

      for (const auto &lit_name : unaddressed_names) {
        auto lit_opt = literal_table_.lookup(lit_name);
        if (!lit_opt.has_value()) {
          continue;
        }

        const Literal &lit = lit_opt.value();
        literal_table_.assign_address(lit_name, location_counter_);

        Line lit_line;
        lit_line.line_number = line_num;
        lit_line.type = LineType::DIRECTIVE;
        lit_line.mnemonic = "*LITERAL*"; // Special marker
        lit_line.operand = lit.name;
        lit_line.address = location_counter_;

        lines_.push_back(lit_line);
        location_counter_ += lit.length;
      }

      lines_.push_back(line);
      return;
    }

    if (dir_type == DirectiveType::END) {
      auto unaddressed_names = literal_table_.get_unaddressed_literal_names();

      for (const auto &lit_name : unaddressed_names) {
        auto lit_opt = literal_table_.lookup(lit_name);
        if (!lit_opt.has_value()) {
          continue;
        }

        const Literal &lit = lit_opt.value();
        literal_table_.assign_address(lit_name, location_counter_);

        Line lit_line;
        lit_line.line_number = line_num;
        lit_line.type = LineType::DIRECTIVE;
        lit_line.mnemonic = "*LITERAL*"; // Special marker
        lit_line.operand = lit.name;
        lit_line.address = location_counter_;

        lines_.push_back(lit_line);
        location_counter_ += lit.length;
      }

      lines_.push_back(line);

      finalize_current_section();
      return;
    }
  }

  if (line.has_label()) {
    std::string label = line.label.value();

    if (label.length() > 6) {
      std::string truncated = label.substr(0, 6);
      warning_handler_.add_symbol_truncated_warning(label, truncated, line_num);
      label = truncated;
      line.label = label;
    }

    // All labels that refer to memory addresses are relocatable
    // (they will be adjusted when the program is loaded)
    Symbol sym(label, location_counter_, true);
    auto result = symbol_table_.insert(sym);
    if (result.is_err()) {
      error_handler_.add_duplicate_symbol_error(label, line_num);
    }
  }

  // Calculate size and increment location counter
  int size = calculate_size(line);
  location_counter_ += size;

  lines_.push_back(line);
}

Line Pass1::parse_line(const std::string &source_line, int line_num) {
  Line line;
  line.line_number = line_num;

  // Trim whitespace
  std::string trimmed = source_line;
  trimmed.erase(0, trimmed.find_first_not_of(" \t"));
  trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

  // Check for comment or empty
  if (trimmed.empty()) {
    line.type = LineType::EMPTY;
    return line;
  }

  if (trimmed[0] == '.') {
    line.type = LineType::COMMENT;
    line.mnemonic = trimmed;
    return line;
  }

  std::istringstream iss(trimmed);
  std::string first, second;

  iss >> first;

  // Check for format 4 prefix (+)
  if (!first.empty() && first[0] == '+') {
    line.is_extended = true;
    first = first.substr(1);
  }

  if (iss >> second) {
    // Check for format 4 prefix on second token
    if (!second.empty() && second[0] == '+') {
      line.is_extended = true;
      second = second.substr(1);
    }

    // Get remaining as operand
    std::string remaining;
    std::getline(iss, remaining);
    remaining.erase(0, remaining.find_first_not_of(" \t"));

    if (!remaining.empty()) {
      // label mnemonic operand
      line.label = first;
      line.mnemonic = second;
      line.operand = remaining;
    } else {
      // mnemonic operand OR label mnemonic
      if (encoder_.is_instruction(first) ||
          string_to_directive(first) != DirectiveType::UNKNOWN) {
        line.mnemonic = first;
        line.operand = second;
      } else {
        line.label = first;
        line.mnemonic = second;
      }
    }
  } else {
    // Only one token - must be mnemonic
    line.mnemonic = first;
  }

  // Classify line type
  classify_line(line);

  // Parse addressing mode for instructions
  if (line.type == LineType::INSTRUCTION && line.has_operand()) {
    parse_addressing_mode(line);
  }

  return line;
}

void Pass1::classify_line(Line &line) {
  // Check if it's a directive
  if (string_to_directive(line.mnemonic) != DirectiveType::UNKNOWN) {
    line.type = LineType::DIRECTIVE;
    return;
  }

  // Check if it's an instruction
  if (encoder_.is_instruction(line.mnemonic)) {
    line.type = LineType::INSTRUCTION;
    return;
  }

  // Unknown - treat as error
  error_handler_.add_invalid_opcode_error(line.mnemonic, line.line_number);
  line.type = LineType::INSTRUCTION; // Set a default to continue processing
}

void Pass1::parse_addressing_mode(Line &line) {
  if (!line.operand.has_value())
    return;

  std::string &op = line.operand.value();

  // Check for indexed addressing (,X) first
  size_t comma_pos = op.find(',');
  if (comma_pos != std::string::npos) {
    std::string index_part = op.substr(comma_pos + 1);
    index_part.erase(0, index_part.find_first_not_of(" \t"));
    if (index_part == "X" || index_part == "x") {
      line.is_indexed = true;
      op = op.substr(0, comma_pos);
    }
  }

  if (!op.empty() && op[0] == '=') {
    line.addressing_mode = AddressingMode::SIMPLE;
    auto parse_result = LiteralParser::parse_literal(op.substr(1));
    if (parse_result.is_ok()) {
      literal_table_.add_literal(op, parse_result.value());
    } else {
      error_handler_.add_syntax_error(parse_result.error().message,
                                      line.line_number);
    }
    return;
  }

  // Check addressing prefix
  if (!op.empty() && op[0] == '#') {
    line.addressing_mode = AddressingMode::IMMEDIATE;
    op = op.substr(1);
  } else if (!op.empty() && op[0] == '@') {
    line.addressing_mode = AddressingMode::INDIRECT;
    op = op.substr(1);
  } else {
    line.addressing_mode = AddressingMode::SIMPLE;
  }
}

int Pass1::calculate_size(const Line &line) {
  // Handle directives
  if (line.type == LineType::DIRECTIVE) {
    auto handler = directive_registry_.get_handler(line.mnemonic);
    if (handler) {
      DirectiveResult result = handler->process_pass1(line, location_counter_);
      if (result.error_message.has_value()) {
        error_handler_.add_syntax_error(result.error_message.value(),
                                        line.line_number);
      }
      return result.bytes_allocated;
    }
    return 0;
  }

  // Handle instructions
  if (line.type == LineType::INSTRUCTION) {
    auto opcode_def = encoder_.get_opcode_definition(line.mnemonic);
    if (opcode_def.has_value()) {
      int format = opcode_def->default_format();

      // If extended format requested, use format 4
      if (line.is_extended) {
        if (!opcode_def->supports_format(4)) {
          error_handler_.add_syntax_error(
              line.mnemonic + " does not support format 4", line.line_number);
          return format; // Return default format anyway
        }
        format = 4;
      }

      return format;
    }
  }

  return 0;
}

void Pass1::start_new_section(const std::string &name, int start_addr) {
  ControlSection section(name, start_addr);
  control_sections_.push_back(section);
  current_section_index_ = control_sections_.size() - 1;

  // Reset state for new section
  location_counter_ = start_addr;
  start_address_ = start_addr;
  extdef_symbols_.clear();
  extref_symbols_.clear();
}

void Pass1::finalize_current_section() {
  if (control_sections_.empty()) {
    return;
  }

  ControlSection &section = control_sections_[current_section_index_];
  section.length = location_counter_ - section.start_address;
  section.extdef_symbols = extdef_symbols_;
  section.extref_symbols = extref_symbols_;
  section.lines = lines_;
  section.symbol_table = symbol_table_;
  section.literal_table = literal_table_;

  // Clear state for next section
  lines_.clear();
  symbol_table_ = SymbolTable();
  literal_table_ = LiteralTable();
}

} // namespace sicxe
