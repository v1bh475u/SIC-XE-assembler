#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace sicxe {

// Forward declarations
struct Line;
class SymbolTable;

// Directive types
enum class DirectiveType {
  START,
  END,
  BYTE,
  WORD,
  RESB,
  RESW,
  BASE,
  NOBASE,
  LTORG,
  EQU,
  ORG,
  UNKNOWN
};

// Helper to convert string to DirectiveType
DirectiveType string_to_directive(const std::string &str);

// Result of directive processing
struct DirectiveResult {
  int bytes_allocated = 0; // How many bytes this directive allocates
  bool should_add_to_symbol_table = false;
  std::optional<std::string> error_message;
};

// Base interface for directive handlers
class DirectiveHandler {
public:
  virtual ~DirectiveHandler() = default;

  // Process directive in pass 1 (calculate size, update location counter)
  virtual DirectiveResult process_pass1(const Line &line,
                                        int current_address) = 0;

  // Generate object code in pass 2
  virtual std::string generate_object_code(const Line &line,
                                           const SymbolTable &symtab) = 0;
};

// Registry for all directives
class DirectiveRegistry {
public:
  DirectiveRegistry();

  // Check if mnemonic is a directive
  bool is_directive(const std::string &mnemonic) const;

  // Get directive type from string
  DirectiveType get_directive_type(const std::string &mnemonic) const;

  // Get handler for a directive (returns shared_ptr, not raw pointer)
  std::shared_ptr<DirectiveHandler>
  get_handler(const std::string &mnemonic) const;

  // Register a new directive
  void register_directive(const std::string &mnemonic,
                          std::unique_ptr<DirectiveHandler> handler);

private:
  void initialize_standard_directives();
  std::unordered_map<std::string, std::shared_ptr<DirectiveHandler>> handlers_;
};

// Concrete directive handlers

class StartDirective : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class EndDirective : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class ByteDirective : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class WordDirective : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class ReserveByte : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class ReserveWord : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class BaseDirective : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class NoBaseDirective : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

class LtorgDirective : public DirectiveHandler {
public:
  DirectiveResult process_pass1(const Line &line, int current_address) override;
  std::string generate_object_code(const Line &line,
                                   const SymbolTable &symtab) override;
};

} // namespace sicxe

#endif // DIRECTIVE_HPP
