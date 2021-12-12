#include "Configuration.h"

Position::Position(int exter, int inter) {
  external = exter;
  interior = inter;
}

bool operator==(const Position &first, const Position &second) {
  return (first.external == second.external) && (first.interior == second.interior);
}

Configuration::Configuration(const Rule &another_rule, const Position &another_pos) : rule(0, std::string()) {
  rule = another_rule;
  pos = another_pos;
}

Configuration::Configuration(const Configuration &another) : rule(0, std::string()) {
  rule = another.rule;
  pos = another.pos;
}

char Configuration::next_step() {
  if (rule.second_part.size() <= pos.interior) {
    return '\0';
  }
  return rule.second_part[pos.interior];
}

bool operator==(const Configuration &first, const Configuration &second) {
  return (first.rule == second.rule) && (first.pos == second.pos);
}