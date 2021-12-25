#include <unordered_set>
#include "Rule.cpp"


struct Configuration {
    Rule rule = Rule('\0', "");
    int pos = 0;
    std::unordered_set<char> next_symbols;

    Configuration() = default;

    Configuration(const Rule &rule_, int pos_, std::unordered_set<char> &next_symbols_);

    Configuration(const Configuration &another);

    char next_step();
};

bool operator==(const Configuration &first, const Configuration &second);
