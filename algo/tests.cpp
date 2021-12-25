#include <gtest/gtest.h>
#include "Earley.cpp"

#pragma once

class AlgoEarleyCase : public ::testing::Test {
};

TEST_F(AlgoEarleyCase, Test1_total) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  std::unordered_map<char, std::vector<Rule>> rules;
  nonterminals.insert('S');
  alphabet.insert('a');
  alphabet.insert('b');
  rules['S'].push_back(Rule('S', "SS"));
  rules['S'].push_back(Rule('S', "aSb"));
  rules['S'].push_back(Rule('S', ""));
  Grammar input_grammar(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(input_grammar);
  ASSERT_EQ(algo.predict("aabbaaabbbab"), "YES");
  ASSERT_EQ(algo.predict("aabbabaaba"), "NO");
  ASSERT_EQ(algo.predict(""), "YES");
  ASSERT_EQ(algo.predict("abbabbba"), "NO");
  ASSERT_EQ(algo.predict("aaaaabbbaabbabaabbbb"), "YES");
  ASSERT_EQ(algo.predict("bbaaabbabba"), "NO");
  ASSERT_EQ(algo.predict("aaaabbaabbbbaaababbb"), "YES");
}

TEST_F(AlgoEarleyCase, Test2_total) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  std::unordered_map<char, std::vector<Rule>> rules;
  nonterminals.insert('S');
  nonterminals.insert('T');
  nonterminals.insert('U');
  alphabet.insert('b');
  alphabet.insert('a');
  rules['S'].push_back(Rule('S', "bT"));
  rules['S'].push_back(Rule('S', "b"));
  rules['T'].push_back(Rule('T', "bUa"));
  rules['T'].push_back(Rule('T', "a"));
  rules['U'].push_back(Rule('U', "Sab"));
  rules['S'].push_back(Rule('U', "b"));
  Grammar input_grammar(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(input_grammar);
  ASSERT_EQ(algo.predict("bbba"), "YES");
  ASSERT_EQ(algo.predict("bbabb"), "NO");
  ASSERT_EQ(algo.predict("bbbaba"), "YES");
  ASSERT_EQ(algo.predict("ababba"), "NO");
  ASSERT_EQ(algo.predict("bbbbbabaaba"), "YES");
  ASSERT_EQ(algo.predict("bbbbbbbabaabaaba"), "YES");
  ASSERT_EQ(algo.predict("bbbbbbbabaabaabaa"), "NO");
}

TEST_F(AlgoEarleyCase, Test3_total) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  std::unordered_map<char, std::vector<Rule>> rules;
  nonterminals.insert('S');
  nonterminals.insert('T');
  nonterminals.insert('V');
  nonterminals.insert('U');
  alphabet.insert('a');
  alphabet.insert('b');
  alphabet.insert('c');
  rules['S'].push_back(Rule('S', "bTa"));
  rules['S'].push_back(Rule('S', "cSa"));
  rules['T'].push_back(Rule('T', "bU"));
  rules['T'].push_back(Rule('T', "aTa"));
  rules['U'].push_back(Rule('U', "V"));
  rules['U'].push_back(Rule('U', "cU"));
  rules['V'].push_back(Rule('V', "caVb"));
  rules['V'].push_back(Rule('V', "a"));
  Grammar input_grammar(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(input_grammar);
  ASSERT_EQ(algo.predict("bbccaaba"), "YES");
  ASSERT_EQ(algo.predict("ccbabcacaabbaaaa"), "YES");
  ASSERT_EQ(algo.predict("ccccbabcacaabbaaaaab"), "NO");
  ASSERT_EQ(algo.predict("baabcacacaabbbaaa"), "YES");
  ASSERT_EQ(algo.predict("abbcabbbacbaaab"), "NO");
  ASSERT_EQ(algo.predict("cccccbaabcacacaabbbaaaaaaaa"), "YES");
  ASSERT_EQ(algo.predict("baacacacacbbbaaa"), "NO");
}

TEST_F(AlgoEarleyCase, Predict_test) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  std::unordered_map<char, std::vector<Rule>> rules;
  nonterminals.insert('S');
  alphabet.insert('a');
  alphabet.insert('b');
  rules['S'].push_back(Rule('S', "SS"));
  rules['S'].push_back(Rule('S', "aSb"));
  rules['S'].push_back(Rule('S', ""));
  Grammar input_grammar(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(input_grammar);
  algo.configurations.resize(6);
  algo.configurations[3]['S'].push_back(Configuration(Rule('S', "aSb"), Position(1, 1)));
  algo.prev_new_configurations = algo.configurations[3];
  ASSERT_TRUE(algo.Predict(3));
  ASSERT_EQ(algo.configurations[3]['a'].size(), 1);
  ASSERT_EQ(algo.configurations[3]['S'].size(), 2);
  ASSERT_EQ(algo.configurations[3]['b'].size(), 0);
  ASSERT_EQ(algo.configurations[3]['a'][0], Configuration(Rule('S', "aSb"), Position(3, 0)));
  ASSERT_EQ(algo.configurations[3]['S'][1], Configuration(Rule('S', "SS"), Position(3, 0)));
  ASSERT_FALSE(algo.Predict(3));
}


TEST_F(AlgoEarleyCase, Scan_test) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  std::unordered_map<char, std::vector<Rule>> rules;
  nonterminals.insert('S');
  alphabet.insert('a');
  alphabet.insert('b');
  rules['S'].push_back(Rule('S', "SS"));
  rules['S'].push_back(Rule('S', "aSb"));
  rules['S'].push_back(Rule('S', ""));
  Grammar input_grammar(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(input_grammar);
  algo.configurations.resize(6);
  algo.configurations[3]['S'].push_back(Configuration(Rule('S', "aSb"), Position(1, 1)));
  algo.prev_new_configurations = algo.configurations[3];
  algo.Predict(3);
  algo.Scan('b', 4);
  ASSERT_EQ(algo.configurations[4].size(), 0);
  algo.Scan('a', 4);
  ASSERT_EQ(algo.configurations[4].size(), 1);
  ASSERT_EQ(algo.configurations[4]['b'].size(), 0);
  ASSERT_EQ(algo.configurations[4]['S'][0], Configuration(Rule('S', "aSb"), Position(3, 1)));
}

TEST_F(AlgoEarleyCase, Complete_test) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  std::unordered_map<char, std::vector<Rule>> rules;
  nonterminals.insert('S');
  alphabet.insert('a');
  alphabet.insert('b');
  rules['S'].push_back(Rule('S', "SS"));
  rules['S'].push_back(Rule('S', "aSb"));
  rules['S'].push_back(Rule('S', ""));
  Grammar input_grammar(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(input_grammar);
  algo.configurations.resize(6);
  algo.configurations[5]['\0'].push_back(Configuration(Rule('S', "aSb"), Position(3, 3)));
  algo.configurations[3]['S'].push_back(Configuration(Rule('S', "SS"), Position(1, 0)));
  algo.prev_new_configurations = algo.configurations[5];
  ASSERT_TRUE(algo.Complete(5));
  ASSERT_EQ(algo.configurations[5]['S'].size(), 1);
  ASSERT_EQ(algo.configurations[1]['S'].size(), 0);
  ASSERT_EQ(algo.configurations[5]['S'][0], Configuration(Rule('S', "SS"), Position(1, 1)));
  ASSERT_FALSE(algo.Complete(5));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}