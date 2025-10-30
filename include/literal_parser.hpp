#ifndef LITERAL_PARSER_HPP
#define LITERAL_PARSER_HPP

#include "error.hpp"
#include "result.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace sicxe {

class LiteralParser {
public:
  static Result<std::vector<uint8_t>, ErrorInfo>
  parse_hex_literal(const std::string &literal);

  static Result<std::vector<uint8_t>, ErrorInfo>
  parse_char_literal(const std::string &literal);

  static bool is_hex_literal(const std::string &str);

  static bool is_char_literal(const std::string &str);

  static bool is_address_literal(const std::string &str);

  static Result<std::vector<uint8_t>, ErrorInfo>
  parse_literal(const std::string &literal);

  static std::string bytes_to_hex(const std::vector<uint8_t> &bytes);
};

} // namespace sicxe

#endif // LITERAL_PARSER_HPP
