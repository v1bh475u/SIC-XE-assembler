#ifndef WARNING_HANDLER_HPP
#define WARNING_HANDLER_HPP

#include <string>
#include <vector>

namespace sicxe {

enum class WarningCode {
  SYMBOL_TRUNCATED,
  PROGRAM_NAME_TRUNCATED,
  GENERAL_WARNING
};

struct Warning {
  WarningCode code;
  std::string message;
  int line_number;
  std::string context;

  Warning(WarningCode c, const std::string &msg, int line = -1,
          const std::string &ctx = "")
      : code(c), message(msg), line_number(line), context(ctx) {}

  std::string to_string() const;
};

class WarningHandler {
public:
  WarningHandler(const std::string &component_name)
      : component_name_(component_name) {}

  virtual ~WarningHandler() = default;

  void add_warning(WarningCode code, const std::string &message,
                   int line_number = -1, const std::string &context = "");

  void add_warning(const Warning &warning);

  [[nodiscard]] bool has_warnings() const { return !warnings_.empty(); }

  [[nodiscard]] size_t warning_count() const { return warnings_.size(); }

  [[nodiscard]] const std::vector<Warning> &get_warnings() const {
    return warnings_;
  }

  void clear();

  [[nodiscard]] std::string format_warnings() const;

  void print_warnings() const;

protected:
  std::string component_name_;
  std::vector<Warning> warnings_;
};

class Pass1WarningHandler : public WarningHandler {
public:
  Pass1WarningHandler() : WarningHandler("Pass 1") {}

  void add_symbol_truncated_warning(const std::string &original,
                                    const std::string &truncated, int line);
  void add_program_name_truncated_warning(const std::string &original,
                                          const std::string &truncated,
                                          int line);
};

class Pass2WarningHandler : public WarningHandler {
public:
  Pass2WarningHandler() : WarningHandler("Pass 2") {}

  void add_program_name_truncated_warning(const std::string &original,
                                          const std::string &truncated);
};

} // namespace sicxe

#endif // WARNING_HANDLER_HPP
