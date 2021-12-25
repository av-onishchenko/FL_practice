#include<string>

#pragma once

struct Rule {
    char first_part = '#';
    std::string second_part;

    Rule(const Rule &another);

    Rule(char first, std::string second);
};


bool operator==(const Rule &first, const Rule &second);
