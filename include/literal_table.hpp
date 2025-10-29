#ifndef LITERAL_TABLE_HPP
#define LITERAL_TABLE_HPP

#include "literal.hpp"
#include <optional>
#include <string>
#include <vector>

namespace sicxe {

class LiteralTable {
public:
  Literal &add_literal(const std::string &name,
                       const std::vector<uint8_t> &value);

  std::optional<Literal> lookup(const std::string &name) const;

  std::optional<int> get_address(const std::string &name) const;

  bool exists(const std::string &name) const;

  std::vector<std::string> get_unaddressed_literal_names() const;

  void assign_address(const std::string &name, int address);

  const std::vector<Literal> &get_all() const { return literals_; }

  void clear() { literals_.clear(); }

private:
  std::vector<Literal> literals_;

  int find_index(const std::string &name) const;
  int find_by_value(const std::vector<uint8_t> &value) const;
};

} // namespace sicxe

#endif // LITERAL_TABLE_HPP
