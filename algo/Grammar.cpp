#include "Grammar.h"

Grammar::Grammar(const std::unordered_set<char> &nt, const std::unordered_set<char> &alp,
                 const std::unordered_map<char, std::vector<Rule>> &rule, char st) {
  nonterminals = nt;
  alphabet = alp;
  rules = rule;
  start = st;
}
