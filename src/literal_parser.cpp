#include "literal_parser.hpp"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace sicxe {

bool LiteralParser::is_hex_literal(const std::string &str) {
  return str.size() >= 3 && str[0] == 'X' && str[1] == '\'' &&
         str.back() == '\'';
}

bool LiteralParser::is_char_literal(const std::string &str) {
  return str.size() >= 3 && str[0] == 'C' && str[1] == '\'' &&
         str.back() == '\'';
}

bool LiteralParser::is_address_literal(const std::string &str) {
  return str == "*";
}

Result<std::vector<uint8_t>, ErrorInfo>
LiteralParser::parse_hex_literal(const std::string &literal) {
  if (!is_hex_literal(literal)) {
    return ErrorInfo(ErrorCode::MALFORMED_LITERAL,
                     "Invalid hex literal format: " + literal);
  }

  // Extract content between quotes: X'09AB' -> 09AB
  std::string hex_str = literal.substr(2, literal.size() - 3);

  // Remove spaces
  hex_str.erase(std::remove_if(hex_str.begin(), hex_str.end(), ::isspace),
                hex_str.end());

  // Validate hex digits
  for (char c : hex_str) {
    if (!std::isxdigit(c)) {
      return ErrorInfo(ErrorCode::MALFORMED_LITERAL,
                       "Invalid hex digit in literal: " + literal);
    }
  }

  // Must have even number of hex digits
  if (hex_str.size() % 2 != 0) {
    return ErrorInfo(ErrorCode::MALFORMED_LITERAL,
                     "Hex literal must have even number of digits: " + literal);
  }

  // Convert to bytes
  std::vector<uint8_t> bytes;
  for (size_t i = 0; i < hex_str.size(); i += 2) {
    std::string byte_str = hex_str.substr(i, 2);
    bytes.push_back(static_cast<uint8_t>(std::stoi(byte_str, nullptr, 16)));
  }

  return bytes;
}

Result<std::vector<uint8_t>, ErrorInfo>
LiteralParser::parse_char_literal(const std::string &literal) {
  if (!is_char_literal(literal)) {
    return ErrorInfo(ErrorCode::MALFORMED_LITERAL,
                     "Invalid character literal format: " + literal);
  }

  // Extract content between quotes: C'hello' -> hello
  std::string content = literal.substr(2, literal.size() - 3);

  // Convert each character to a byte
  std::vector<uint8_t> bytes;
  for (char c : content) {
    bytes.push_back(static_cast<uint8_t>(c));
  }

  return bytes;
}

Result<std::vector<uint8_t>, ErrorInfo>
LiteralParser::parse_literal(const std::string &literal) {
  if (is_hex_literal(literal)) {
    return parse_hex_literal(literal);
  } else if (is_char_literal(literal)) {
    return parse_char_literal(literal);
  } else if (is_address_literal(literal)) {
    // For =*, return a placeholder (will be replaced with actual address later)
    // Using 3 bytes for a word (address)
    return std::vector<uint8_t>{0x00, 0x00, 0x00};
  } else {
    return ErrorInfo(ErrorCode::MALFORMED_LITERAL,
                     "Unknown literal format: " + literal);
  }
}

std::string LiteralParser::bytes_to_hex(const std::vector<uint8_t> &bytes) {
  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setfill('0');

  for (uint8_t byte : bytes) {
    oss << std::setw(2) << static_cast<int>(byte);
  }

  return oss.str();
}

} // namespace sicxe
