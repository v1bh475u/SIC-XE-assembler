#include "literal_table.hpp"

namespace sicxe {

Literal &LiteralTable::add_literal(const std::string &name,
                                   const std::vector<uint8_t> &value) {
  // Check if literal with same value already exists (deduplication by value)
  int idx = find_by_value(value);
  if (idx != -1) {
    return literals_[idx];
  }

  // Add new literal
  literals_.emplace_back(name, value);
  return literals_.back();
}

std::optional<Literal> LiteralTable::lookup(const std::string &name) const {
  int idx = find_index(name);
  if (idx != -1) {
    return literals_[idx];
  }
  return std::nullopt;
}

std::optional<int> LiteralTable::get_address(const std::string &name) const {
  auto lit = lookup(name);
  if (lit.has_value() && lit->has_address()) {
    return lit->address;
  }
  return std::nullopt;
}

bool LiteralTable::exists(const std::string &name) const {
  return find_index(name) != -1;
}

std::vector<Literal *> LiteralTable::get_unaddressed_literals() {
  std::vector<Literal *> unaddressed;
  for (auto &lit : literals_) {
    if (!lit.has_address()) {
      unaddressed.push_back(&lit);
    }
  }
  return unaddressed;
}

void LiteralTable::assign_address(const std::string &name, int address) {
  int idx = find_index(name);
  if (idx != -1) {
    literals_[idx].address = address;
  }
}

int LiteralTable::find_index(const std::string &name) const {
  for (size_t i = 0; i < literals_.size(); ++i) {
    if (literals_[i].name == name) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

int LiteralTable::find_by_value(const std::vector<uint8_t> &value) const {
  for (size_t i = 0; i < literals_.size(); ++i) {
    if (literals_[i].value == value) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

} // namespace sicxe
