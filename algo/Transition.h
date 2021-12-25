#include <string>
#include "Rule.cpp"

#pragma once

struct Transition {
    std::string type;
    int state;
    Rule rule;

    Transition();

    Transition(const std::string &type_, const Rule &rule_);

    Transition(const std::string &type_, char state_);

    Transition(const Transition &another);
};
