#include "Transition.h"


Transition::Transition() : rule(0, std::string()) {
  type = "none";
  state = 0;
}

Transition::Transition(const std::string &type_, const Rule &rule_) : rule(0, std::string()) {
  type = type_;
  rule = rule_;
}

Transition::Transition(const std::string &type_, char state_) : rule(0, std::string()) {
  type = type_;
  state = state_;
}

Transition::Transition(const Transition &another) : rule(0, std::string()) {
  type = another.type;
  state = another.state;
  rule = another.rule;
}