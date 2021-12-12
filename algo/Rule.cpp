#pragma once
#include "Rule.h"

Rule::Rule(const Rule &another) {
  first_part = another.first_part;
  second_part = another.second_part;
}

Rule::Rule(char first, const std::string &second) {
  first_part = first;
  second_part = second;
}

bool operator==(const Rule &first, const Rule &second) {
  return (first.first_part == second.first_part) && (first.second_part == second.second_part);
}