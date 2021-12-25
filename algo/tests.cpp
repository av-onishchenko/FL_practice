#include <gtest/gtest.h>
#include "LR1.cpp"


class AlgoLR1Case : public ::testing::Test {
};

TEST_F(AlgoLR1Case, Test1_total) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('A');
  alphabet.insert('a');
  alphabet.insert('b');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "bAb"));
  rules['S'].push_back(Rule('S', "Ab"));
  rules['A'].push_back(Rule('A', "a"));
  rules['A'].push_back(Rule('A', "aa"));
  rules['A'].push_back(Rule('A', "aaa"));
  rules['A'].push_back(Rule('A', ""));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(g);
  ASSERT_EQ(algo.predict("bab"), "YES");
  ASSERT_EQ(algo.predict("aaab"), "YES");
  ASSERT_EQ(algo.predict("bb"), "YES");
  ASSERT_EQ(algo.predict("baab"), "YES");
  ASSERT_EQ(algo.predict("baaaab"), "NO");
  ASSERT_EQ(algo.predict(""), "NO");
  ASSERT_EQ(algo.predict("baa"), "NO");
  ASSERT_EQ(algo.predict("aaabb"), "NO");
}

TEST_F(AlgoLR1Case, Test2_total) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('A');
  nonterminals.insert('B');
  alphabet.insert('a');
  alphabet.insert('b');
  alphabet.insert('c');
  alphabet.insert('d');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "AS"));
  rules['S'].push_back(Rule('S', "A"));
  rules['S'].push_back(Rule('S', "B"));
  rules['A'].push_back(Rule('A', "aAb"));
  rules['A'].push_back(Rule('A', ""));
  rules['B'].push_back(Rule('B', "cBd"));
  rules['B'].push_back(Rule('B', ""));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(g);
  ASSERT_EQ(algo.predict("aaaabbbb"), "YES");
  ASSERT_EQ(algo.predict("abccdd"), "YES");
  ASSERT_EQ(algo.predict("aaabbbcccccddddd"), "YES");
  ASSERT_EQ(algo.predict(""), "YES");
  ASSERT_EQ(algo.predict("aaabbbccaccdddddaabbcccddd"), "NO");
  ASSERT_EQ(algo.predict("ccccddd"), "NO");
  ASSERT_EQ(algo.predict("cccdddaabb"), "NO");
  ASSERT_EQ(algo.predict("aaabbbcccacddddd"), "NO");
}

TEST_F(AlgoLR1Case, Test3_total) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('A');
  nonterminals.insert('B');
  alphabet.insert('a');
  alphabet.insert('b');
  alphabet.insert('c');
  alphabet.insert('d');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "SaSb"));
  rules['S'].push_back(Rule('S', "Sa"));
  rules['S'].push_back(Rule('S', "Bc"));
  rules['S'].push_back(Rule('S', "A"));
  rules['A'].push_back(Rule('A', "dB"));
  rules['B'].push_back(Rule('B', ""));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(g);
  ASSERT_EQ(algo.predict("cacb"), "YES");
  ASSERT_EQ(algo.predict("caaaaaaaaacb"), "YES");
  ASSERT_EQ(algo.predict("dacaaaaaaaaaaaaaab"), "YES");
  ASSERT_EQ(algo.predict("daaaaaaaaaadaaaaaaaaaaaab"), "YES");
  ASSERT_EQ(algo.predict("dbadb"), "NO");
  ASSERT_EQ(algo.predict("caaaaaaaaaac"), "NO");
  ASSERT_EQ(algo.predict("dacaaaaaaaaaaaaaabbb"), "NO");
  ASSERT_EQ(algo.predict("caaaaaaaaaaddaaaaaaaaaaaab"), "NO");
}

TEST_F(AlgoLR1Case, Test_make_table) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('A');
  alphabet.insert('a');
  alphabet.insert('b');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "bAb"));
  rules['S'].push_back(Rule('S', "Ab"));
  rules['A'].push_back(Rule('A', "a"));
  rules['A'].push_back(Rule('A', "aa"));
  rules['A'].push_back(Rule('A', "aaa"));
  rules['A'].push_back(Rule('A', ""));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit(g);
  ASSERT_EQ(algo.table.size(), 10);
  ASSERT_EQ(algo.table[0]['a'].type, "shift");
  ASSERT_EQ(algo.table[9]['b'].type, "reduce");
  ASSERT_EQ(algo.table[4][algo.EndOfWord].type, "accept");
  ASSERT_EQ(algo.table[3]['a'].type, "none");
  ASSERT_EQ(algo.table[7][algo.EndOfWord].rule, Rule('S', "Ab"));
  ASSERT_EQ(algo.table[0]['A'].state, 3);
}

TEST_F(AlgoLR1Case, Test_add_state) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('D');
  alphabet.insert('b');
  alphabet.insert('d');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "D"));
  rules['D'].push_back(Rule('D', "Db"));
  rules['D'].push_back(Rule('D', "d"));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit_grammar(g);
  algo.prepare_states();
  std::unordered_set<char> step;
  step.insert('D');
  algo.add_state(step, 0);
  ASSERT_EQ(algo.states.size(), 2);
  ASSERT_EQ(algo.states[1].size(), 3);
  ASSERT_EQ(algo.states[1][2].rule, Rule('D', "Db"));
  ASSERT_EQ(algo.states[1][2].next_symbols.size(), 1);
  ASSERT_EQ(*(algo.states[1][2].next_symbols.begin()), 'b');
  step.erase('D');
  step.insert('b');
  algo.add_state(step, 1);
  ASSERT_EQ(algo.states.size(), 3);
  ASSERT_EQ(algo.states[2].size(), 2);
  ASSERT_EQ(algo.states[2][0].rule, Rule('D', "Db"));
  ASSERT_EQ(algo.states[2][0].pos, 2);
  algo.add_state(step, 2);
  ASSERT_EQ(algo.states.size(), 3);
  step.insert('D');
  algo.add_state(step, 0);
  ASSERT_EQ(algo.states.size(), 3);
}

TEST_F(AlgoLR1Case, Test_go_to) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('A');
  alphabet.insert('a');
  alphabet.insert('b');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "bAb"));
  rules['S'].push_back(Rule('S', "Ab"));
  rules['A'].push_back(Rule('A', "a"));
  rules['A'].push_back(Rule('A', "aa"));
  rules['A'].push_back(Rule('A', "aaa"));
  rules['A'].push_back(Rule('A', ""));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit_grammar(g);
  algo.prepare_states();
  std::vector<Configuration> new_configuration = algo.go_to(algo.states[0], 'A');
  ASSERT_EQ(new_configuration.size(), 1);
  ASSERT_EQ(new_configuration[0].rule, Rule('S', "Ab"));
  new_configuration = algo.go_to(new_configuration, 'b');
  ASSERT_EQ(new_configuration.size(), 1);
  ASSERT_EQ(new_configuration[0].pos, 2);
  ASSERT_EQ(*(new_configuration[0].next_symbols.begin()), '$');
  new_configuration = algo.go_to(algo.states[0], 'a');
  ASSERT_EQ(new_configuration.size(), 3);
  ASSERT_EQ(new_configuration[1].rule, Rule('A', "aa"));
  ASSERT_EQ(new_configuration[1].pos, 1);
  ASSERT_EQ(*(new_configuration[1].next_symbols.begin()), 'b');
}

TEST_F(AlgoLR1Case, Test_reduce) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('D');
  alphabet.insert('b');
  alphabet.insert('d');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "D"));
  rules['D'].push_back(Rule('D', "Db"));
  rules['D'].push_back(Rule('D', "d"));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit_grammar(g);
  algo.prepare_states();
  std::unordered_set<char> step;
  step.insert('D');
  algo.add_state(step, 0);
  algo.reduce(1);
  ASSERT_EQ(algo.table[1]['$'].type, "reduce");
  ASSERT_EQ(algo.table[1]['$'].rule, Rule('S', "D"));
  ASSERT_EQ(algo.table[1]['b'].type, "none");
  step.erase('D');
  step.insert('b');
  algo.add_state(step, 1);
  algo.reduce(2);
  ASSERT_EQ(algo.table[2]['b'].type, "reduce");
  ASSERT_EQ(algo.table[2]['b'].rule, Rule('D', "Db"));
  step.erase('b');
  step.insert('S');
  algo.add_state(step, 0);
  algo.reduce(3);
  ASSERT_EQ(algo.table[3]['$'].type, "accept");
}

TEST_F(AlgoLR1Case, Test_closure) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('A');
  nonterminals.insert('B');
  alphabet.insert('a');
  alphabet.insert('b');
  alphabet.insert('c');
  alphabet.insert('d');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "AS"));
  rules['S'].push_back(Rule('S', "A"));
  rules['S'].push_back(Rule('S', "B"));
  rules['A'].push_back(Rule('A', "aAb"));
  rules['A'].push_back(Rule('A', ""));
  rules['B'].push_back(Rule('B', "cBd"));
  rules['B'].push_back(Rule('B', ""));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit_grammar(g);
  std::vector<Configuration> new_config;
  std::unordered_set<char> next;
  next.insert('$');
  new_config.push_back(Configuration(rules['S'][0], 0, next));
  algo.closure(new_config);
  ASSERT_EQ(new_config.size(), 3);
  ASSERT_EQ(new_config[1].rule, Rule('A', "aAb"));
  ASSERT_EQ(new_config[2].rule, Rule('A', ""));
  ASSERT_EQ(new_config[1].pos, 0);
  ASSERT_EQ(new_config[1].next_symbols.size(), 2);
  ASSERT_EQ(*(new_config[1].next_symbols.begin()), 'c');
  ASSERT_EQ(*(++new_config[1].next_symbols.begin()), 'a');
  algo.closure(new_config);
  ASSERT_EQ(new_config.size(), 3);
  std::vector<Configuration> second_config;
  second_config.push_back(new_config[2]);
  ASSERT_EQ(second_config.size(), 1);
}

TEST_F(AlgoLR1Case, Test_make_first) {
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> alphabet;
  nonterminals.insert('S');
  nonterminals.insert('A');
  nonterminals.insert('B');
  alphabet.insert('a');
  alphabet.insert('b');
  alphabet.insert('c');
  alphabet.insert('d');
  std::unordered_map<char, std::vector<Rule>> rules;
  rules['S'].push_back(Rule('S', "AS"));
  rules['S'].push_back(Rule('S', "A"));
  rules['S'].push_back(Rule('S', "B"));
  rules['A'].push_back(Rule('A', "aAb"));
  rules['A'].push_back(Rule('A', ""));
  rules['B'].push_back(Rule('B', "cBd"));
  rules['B'].push_back(Rule('B', ""));
  Grammar g(nonterminals, alphabet, rules, 'S');
  Algo algo = Algo();
  algo.fit_grammar(g);
  std::unordered_set<char> used_first;
  std::unordered_set<char> first = algo.make_first('S', used_first);
  ASSERT_EQ(first.size(), 2);
  std::unordered_set<char> next;
  next.insert('c');
  next.insert('a');
  ASSERT_EQ(first, next);
  std::unordered_set<char> used_second;
  first = algo.make_first('B', used_second);
  ASSERT_EQ(first.size(), 1);
  ASSERT_EQ(*(first.begin()), 'c');
  std::unordered_set<char> used_third;
  first = algo.make_first('A', used_second);
  ASSERT_EQ(first.size(), 1);
  ASSERT_EQ(*(first.begin()), 'a');
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}