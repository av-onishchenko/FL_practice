#include <vector>
#include <unordered_map>
#pragma once

#include <string>
#include <unordered_set>

struct Rule {
    char first_part = '#';
    std::string second_part;

    Rule(const Rule &another);

    Rule(char first, const std::string &second);
};


bool operator==(const Rule &first, const Rule &second);