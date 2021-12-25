#include "LR1.h"


void Algo::merge(std::unordered_set<char> &helper, std::unordered_set<char> &res) {
  for (auto it = helper.begin(); it != helper.end(); ++it) {
    res.insert(*it);
  }
}

std::unordered_set<char> Algo::make_first(char step, std::unordered_set<char> &used) {
  std::unordered_set<char> ans;
  if (grammar.alphabet.count(step)) {
    ans.insert(step);
    return ans;
  }
  used.insert(step);
  for (int i = 0; i < grammar.rules[step].size(); ++i) {
    if (used.count(grammar.rules[step][i].second_part[0])) {
      continue;
    }
    std::unordered_set<char> next_first = make_first(grammar.rules[step][i].second_part[0], used);
    merge(next_first, ans);
  }
  return ans;
}

void Algo::update_state(std::vector<Configuration> &state, Configuration &conf) {
  for (int i = 0; i < state.size(); ++i) {
    if (state[i] == conf) {
      return;
    }
  }
  state.push_back(conf);
}

void Algo::closure(std::vector<Configuration> &state) {
  for (int i = 0; i < state.size(); ++i) {
    if (!grammar.nonterminals.count(state[i].next_step())) {
      continue;
    }
    std::unordered_set<char> first;
    if (state[i].rule.second_part.size() - 1 == state[i].pos) {
      first = state[i].next_symbols;
    } else {
      std::unordered_set<char> used;
      first = make_first(state[i].rule.second_part[state[i].pos + 1], used);
    }
    for (int j = 0; j < grammar.rules[state[i].next_step()].size(); ++j) {
      Configuration new_conf = Configuration(grammar.rules[state[i].next_step()][j], 0, first);
      update_state(state, new_conf);
    }
  }
}

void Algo::reduce(int pos) {
  for (int i = 0; i < states[pos].size(); ++i) {
    if (states[pos][i].rule.second_part.size() != states[pos][i].pos) {
      continue;
    }
    if (states[pos][i].rule.first_part == grammar.start) {
      Transition new_transition("accept", 0);
      table[pos][EndOfWord] = new_transition;
    } else {
      for (auto it = states[pos][i].next_symbols.begin(); it != states[pos][i].next_symbols.end(); ++it) {
        Transition new_transition("reduce", states[pos][i].rule);
        table[pos][*it] = new_transition;
      }
    }
  }
}

std::vector<Configuration> Algo::go_to(std::vector<Configuration> &state, char step) {
  std::vector<Configuration> ans;
  for (int i = 0; i < state.size(); ++i) {
    if (state[i].next_step() == step) {
      Configuration new_conf(state[i].rule, state[i].pos + 1, state[i].next_symbols);
      ans.push_back(new_conf);
    }
  }
  closure(ans);
  return ans;
}

int Algo::check_new(std::vector<Configuration> &state) {
  for (int pos = 0; pos < states.size(); ++pos) {
    int count = 0;
    for (int i = 0; i < state.size(); ++i) {
      for (int j = 0; j < states[pos].size(); ++j) {
        if (state[i] == states[pos][j]) {
          ++count;
          break;
        }
      }
    }
    if (count == state.size()) {
      return pos;
    }
  }
  return -1;
}

void Algo::add_state(std::unordered_set<char> &symbols, int pos) {
  for (auto it = symbols.begin(); it != symbols.end(); ++it) {
    std::vector<Configuration> new_conf = go_to(states[pos], *it);
    if (!new_conf.size()) {
      continue;
    }
    int state_pos = check_new(new_conf);
    if (state_pos == -1) {
      states.push_back(new_conf);
      table.resize(states.size());
      state_pos = states.size() - 1;
    }
    Transition new_trans("shift", state_pos);
    table[pos][*it] = new_trans;
  }
}

void Algo::make_table() {
  table.resize(1);
  for (int i = 0; i < states.size(); ++i) {
    reduce(i);
    add_state(grammar.alphabet, i);
    add_state(grammar.nonterminals, i);
  }
}

void Algo::fit_grammar(const Grammar &input_gram) {
  grammar = input_gram;
  std::string first_left = std::string(1, grammar.start);
  grammar.rules['#'].push_back(Rule('#', first_left));
  grammar.nonterminals.insert('#');
  grammar.start = '#';
  grammar.alphabet.insert(EndOfWord);
}

void Algo::prepare_states() {
  std::vector<Configuration> first_conf(1);
  std::unordered_set<char> first_next;
  first_next.insert(EndOfWord);
  Configuration first(grammar.rules[grammar.start][0], 0, first_next);
  first_conf[0] = first;
  closure(first_conf);
  states.push_back(first_conf);
}


void Algo::fit(const Grammar &input_gram) {
  fit_grammar(input_gram);
  prepare_states();
  make_table();
}

std::string Algo::predict(std::string word) {
  std::stack<int> Stack;
  Stack.push(0);
  word += EndOfWord;
  char curr_char = 0;
  while (curr_char < word.size()) {
    Transition step = table[Stack.top()][word[curr_char]];
    if (step.type == "accept") {
      return "YES";
    }
    if (step.type == "reduce") {
      for (int i = 0; i < step.rule.second_part.size(); ++i) {
        Stack.pop();
      }
      Stack.push(table[Stack.top()][step.rule.first_part].state);
    } else if (step.type == "shift") {
      Stack.push(step.state);
      ++curr_char;
    } else {
      return "NO";
    }
  }
}
