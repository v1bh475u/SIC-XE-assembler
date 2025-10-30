#include "directive.hpp"
#include "error.hpp"
#include "line.hpp"
#include "literal_parser.hpp"
#include "symbol_table.hpp"
#include <iomanip>
#include <sstream>

namespace sicxe {

// Helper to convert string to DirectiveType
DirectiveType string_to_directive(const std::string &str) {
  static const std::unordered_map<std::string, DirectiveType> directive_map = {
      {"START", DirectiveType::START},   {"END", DirectiveType::END},
      {"BYTE", DirectiveType::BYTE},     {"WORD", DirectiveType::WORD},
      {"RESB", DirectiveType::RESB},     {"RESW", DirectiveType::RESW},
      {"BASE", DirectiveType::BASE},     {"NOBASE", DirectiveType::NOBASE},
      {"LTORG", DirectiveType::LTORG},   {"EQU", DirectiveType::EQU},
      {"ORG", DirectiveType::ORG},       {"CSECT", DirectiveType::CSECT},
      {"EXTDEF", DirectiveType::EXTDEF}, {"EXTREF", DirectiveType::EXTREF},
      {"USE", DirectiveType::USE}};

  auto it = directive_map.find(str);
  return it != directive_map.end() ? it->second : DirectiveType::UNKNOWN;
}

// DirectiveRegistry implementation

DirectiveRegistry::DirectiveRegistry() { initialize_standard_directives(); }

void DirectiveRegistry::initialize_standard_directives() {
  register_directive("START", std::make_unique<StartDirective>());
  register_directive("END", std::make_unique<EndDirective>());
  register_directive("BYTE", std::make_unique<ByteDirective>());
  register_directive("WORD", std::make_unique<WordDirective>());
  register_directive("RESB", std::make_unique<ReserveByte>());
  register_directive("RESW", std::make_unique<ReserveWord>());
  register_directive("BASE", std::make_unique<BaseDirective>());
  register_directive("NOBASE", std::make_unique<NoBaseDirective>());
  register_directive("LTORG", std::make_unique<LtorgDirective>());
  register_directive("ORG", std::make_unique<OrgDirective>());
  register_directive("EQU", std::make_unique<EquDirective>());
  register_directive("CSECT", std::make_unique<CsectDirective>());
  register_directive("EXTDEF", std::make_unique<ExtdefDirective>());
  register_directive("EXTREF", std::make_unique<ExtrefDirective>());
  register_directive("USE", std::make_unique<UseDirective>());
}

bool DirectiveRegistry::is_directive(const std::string &mnemonic) const {
  return handlers_.find(mnemonic) != handlers_.end();
}

DirectiveType
DirectiveRegistry::get_directive_type(const std::string &mnemonic) const {
  return string_to_directive(mnemonic);
}

std::shared_ptr<DirectiveHandler>
DirectiveRegistry::get_handler(const std::string &mnemonic) const {
  auto it = handlers_.find(mnemonic);
  return it != handlers_.end() ? it->second : nullptr;
}

void DirectiveRegistry::register_directive(
    const std::string &mnemonic, std::unique_ptr<DirectiveHandler> handler) {
  handlers_[mnemonic] = std::move(handler);
}

// START directive

DirectiveResult StartDirective::process_pass1(const Line &line,
                                              int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // START doesn't allocate space
  result.should_add_to_symbol_table = false;
  return result;
}

std::string StartDirective::generate_object_code(const Line &line,
                                                 const SymbolTable &symtab) {
  return ""; // START generates no object code
}

// END directive

DirectiveResult EndDirective::process_pass1(const Line &line,
                                            int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0;
  result.should_add_to_symbol_table = false;
  return result;
}

std::string EndDirective::generate_object_code(const Line &line,
                                               const SymbolTable &symtab) {
  return ""; // END generates no object code
}

// BYTE directive

DirectiveResult ByteDirective::process_pass1(const Line &line,
                                             int current_address) {
  DirectiveResult result;

  if (!line.operand.has_value()) {
    result.error_message = "BYTE directive requires an operand";
    return result;
  }

  auto parse_result = LiteralParser::parse_literal(line.operand.value());
  if (parse_result.is_err()) {
    result.error_message = parse_result.error().message;
  } else {
    result.bytes_allocated = parse_result.value().size();
    result.should_add_to_symbol_table = line.label.has_value();
  }

  return result;
}

std::string ByteDirective::generate_object_code(const Line &line,
                                                const SymbolTable &symtab) {
  if (!line.operand.has_value()) {
    // Return empty string on error - error will be caught elsewhere
    return "";
  }

  auto parse_result = LiteralParser::parse_literal(line.operand.value());
  if (parse_result.is_err()) {
    // Return empty string on error
    return "";
  }

  return LiteralParser::bytes_to_hex(parse_result.value());
}

// WORD directive

DirectiveResult WordDirective::process_pass1(const Line &line,
                                             int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 3; // WORD is always 3 bytes in SIC/XE
  result.should_add_to_symbol_table = line.label.has_value();
  return result;
}

std::string WordDirective::generate_object_code(const Line &line,
                                                const SymbolTable &symtab) {
  if (!line.operand.has_value()) {
    // Return empty string on error
    return "";
  }

  try {
    int value = std::stoi(line.operand.value());
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0') << std::setw(6)
        << (value & 0xFFFFFF);
    return oss.str();
  } catch (...) {
    // Return empty string on error
    return "";
  }
}

// RESB (Reserve Byte)

DirectiveResult ReserveByte::process_pass1(const Line &line,
                                           int current_address) {
  DirectiveResult result;

  if (!line.operand.has_value()) {
    result.error_message = "RESB directive requires an operand";
    return result;
  }

  try {
    int count = std::stoi(line.operand.value());
    if (count < 0) {
      result.error_message = "RESB count must be non-negative";
      return result;
    }
    result.bytes_allocated = count;
    result.should_add_to_symbol_table = line.label.has_value();
  } catch (...) {
    result.error_message = "RESB operand must be an integer";
  }

  return result;
}

std::string ReserveByte::generate_object_code(const Line &line,
                                              const SymbolTable &symtab) {
  return ""; // RESB generates no object code (just reserves space)
}

// RESW (Reserve Word)

DirectiveResult ReserveWord::process_pass1(const Line &line,
                                           int current_address) {
  DirectiveResult result;

  if (!line.operand.has_value()) {
    result.error_message = "RESW directive requires an operand";
    return result;
  }

  try {
    int count = std::stoi(line.operand.value());
    if (count < 0) {
      result.error_message = "RESW count must be non-negative";
      return result;
    }
    result.bytes_allocated = count * 3; // Each word is 3 bytes
    result.should_add_to_symbol_table = line.label.has_value();
  } catch (...) {
    result.error_message = "RESW operand must be an integer";
  }

  return result;
}

std::string ReserveWord::generate_object_code(const Line &line,
                                              const SymbolTable &symtab) {
  return ""; // RESW generates no object code (just reserves space)
}

DirectiveResult BaseDirective::process_pass1(const Line &line,
                                             int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // BASE doesn't allocate space
  result.should_add_to_symbol_table = false;

  if (!line.operand.has_value()) {
    result.error_message = "BASE directive requires an operand (symbol name)";
  }

  return result;
}

std::string BaseDirective::generate_object_code(const Line &line,
                                                const SymbolTable &symtab) {
  return ""; // BASE generates no object code (just sets register)
}

// NOBASE directive

DirectiveResult NoBaseDirective::process_pass1(const Line &line,
                                               int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // NOBASE doesn't allocate space
  result.should_add_to_symbol_table = false;
  return result;
}

std::string NoBaseDirective::generate_object_code(const Line &line,
                                                  const SymbolTable &symtab) {
  return ""; // NOBASE generates no object code (just clears register)
}

DirectiveResult LtorgDirective::process_pass1(const Line &line,
                                              int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // LTORG doesn't allocate space itself
  result.should_add_to_symbol_table = false;
  return result;
}

std::string LtorgDirective::generate_object_code(const Line &line,
                                                 const SymbolTable &symtab) {
  return ""; // LTORG generates no object code directly (literals do)
}

DirectiveResult OrgDirective::process_pass1(const Line &line,
                                            int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // ORG doesn't allocate space itself
  result.should_add_to_symbol_table = false;
  return result;
}

std::string OrgDirective::generate_object_code(const Line &line,
                                               const SymbolTable &symtab) {
  return ""; // ORG generates no object code
}

DirectiveResult EquDirective::process_pass1(const Line &line,
                                            int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // EQU doesn't allocate space
  result.should_add_to_symbol_table =
      false; // We handle symbol insertion manually
  return result;
}

std::string EquDirective::generate_object_code(const Line &line,
                                               const SymbolTable &symtab) {
  return ""; // EQU generates no object code
}

DirectiveResult CsectDirective::process_pass1(const Line &line,
                                              int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // CSECT doesn't allocate space
  result.should_add_to_symbol_table = false;
  return result;
}

std::string CsectDirective::generate_object_code(const Line &line,
                                                 const SymbolTable &symtab) {
  return ""; // CSECT generates no object code
}

DirectiveResult ExtdefDirective::process_pass1(const Line &line,
                                               int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // EXTDEF doesn't allocate space
  result.should_add_to_symbol_table = false;
  return result;
}

std::string ExtdefDirective::generate_object_code(const Line &line,
                                                  const SymbolTable &symtab) {
  return ""; // EXTDEF generates no object code
}

DirectiveResult ExtrefDirective::process_pass1(const Line &line,
                                               int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0; // EXTREF doesn't allocate space
  result.should_add_to_symbol_table = false;
  return result;
}

std::string ExtrefDirective::generate_object_code(const Line &line,
                                                  const SymbolTable &symtab) {
  return ""; // EXTREF generates no object code
}

// USE directive - not supported, generate error

DirectiveResult UseDirective::process_pass1(const Line &line,
                                            int current_address) {
  DirectiveResult result;
  result.bytes_allocated = 0;
  result.should_add_to_symbol_table = false;
  result.error_message = "USE directive is not supported. This assembler does "
                         "not support control sections with USE.";
  return result;
}

std::string UseDirective::generate_object_code(const Line &line,
                                               const SymbolTable &symtab) {
  return ""; // USE generates no object code (error already reported)
}

} // namespace sicxe
