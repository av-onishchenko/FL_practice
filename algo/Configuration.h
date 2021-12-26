#pragma once

#include "Rule.cpp"

struct Position {
    int external;
    int interior;

    Position() = default;

    Position(const Position &another);

    Position(int exter, int inter);
};

bool operator==(const Position &first, const Position &second);

struct Configuration {
    Position pos;
    Rule rule;

    Configuration(const Rule &another_rule, const Position &another_pos);

    Configuration(const Configuration &another);

    char next_step();
};

bool operator==(const Configuration &first, const Configuration &second);