#include "pass1.hpp"
#include "directive.hpp"
#include "error_handler.hpp"
#include <fstream>
#include <sstream>

namespace sicxe {

Pass1::Pass1(const OpcodeEncoder &encoder)
    : encoder_(encoder), location_counter_(0), start_address_(0),
      program_length_(0) {}

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
      if (line.has_operand()) {
        try {
          location_counter_ = std::stoi(line.operand.value(), nullptr, 16);
          start_address_ = location_counter_;
          line.address = location_counter_;
        } catch (...) {
          error_handler_.add_invalid_start_address_error(line_num);
        }
      }
      lines_.push_back(line);
      return;
    }

    // Handle END directive
    if (dir_type == DirectiveType::END) {
      lines_.push_back(line);
      return;
    }
  }

  // Add label to symbol table if present
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

} // namespace sicxe
