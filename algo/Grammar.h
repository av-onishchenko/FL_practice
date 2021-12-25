#include<unordered_set>
#include<unordered_map>
#include"Rule.cpp"


struct Grammar {
    std::unordered_set<char> nonterminals;
    std::unordered_set<char> alphabet;
    std::unordered_map<char, std::vector<Rule>> rules;
    char start;

    Grammar() = default;

    Grammar(const std::unordered_set<char> &nt, const std::unordered_set<char> &alp,
            const std::unordered_map<char, std::vector<Rule>> &rule, char st);
};

