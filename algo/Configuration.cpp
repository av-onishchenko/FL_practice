#include "Configuration.h"


Configuration::Configuration(const Rule &rule_, int pos_, std::unordered_set<char> &next_symbols_) {
  rule = rule_;
  pos = pos_;
  next_symbols = next_symbols_;
}

char Configuration::next_step() {
  if (rule.second_part.size() <= pos) {
    return '\0';
  }
  return rule.second_part[pos];
}


Configuration::Configuration(const Configuration &another) : rule(0, std::string()) {
  rule = another.rule;
  pos = another.pos;
  next_symbols = another.next_symbols;
}

bool operator==(const Configuration &first, const Configuration &second) {
  return (first.pos == second.pos) && (first.rule == second.rule) && (first.next_symbols == second.next_symbols);
}
