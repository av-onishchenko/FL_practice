#include "Earley.h"

#pragma once


bool Algo::add(std::vector<Configuration> &confs, Configuration &new_conf) {
  for (int i = 0; i < confs.size(); ++i) {
    if (confs[i] == new_conf) {
      return false;
    }
  }
  confs.push_back(new_conf);
  return true;
}

void Algo::Scan(char next_symbol, int pos) {
  for (int i = 0; i < configurations[pos - 1][next_symbol].size(); ++i) {
    Configuration new_conf = configurations[pos - 1][next_symbol][i];
    ++new_conf.pos.interior;
    char next_step = new_conf.next_step();
    add(configurations[pos][next_step], new_conf);
  }
}

bool Algo::Predict(int pos) {
  bool change = false;
  for (auto it = prev_new_configurations.begin(); it != prev_new_configurations.end(); ++it) {
    char next_nonterm = (*it).first;
    if (!grammar.nonterminals.count(next_nonterm)) { continue; };
    for (int k = 0; k < grammar.rules[next_nonterm].size(); ++k) {
      char next_step = grammar.rules[next_nonterm][k].second_part[0];
      Configuration new_conf = Configuration(grammar.rules[next_nonterm][k], Position(pos, 0));
      if (add(configurations[pos][next_step], new_conf)) {
        add(new_configurations[next_step], new_conf);
        if (add(prev_new_configurations[next_step], new_conf)) {
          change = true;
        }
      }
    }
  }
  return change;
}

bool Algo::Complete(int pos) {
  bool change = false;
  for (int i = 0; i < prev_new_configurations[EndOfWord].size(); ++i) {
    int prev_pos = prev_new_configurations[EndOfWord][i].pos.external;
    char prev_step = prev_new_configurations[EndOfWord][i].rule.first_part;
    for (int j = 0; j < configurations[prev_pos][prev_step].size(); ++j) {
      Configuration new_conf = configurations[prev_pos][prev_step][j];
      ++new_conf.pos.interior;
      char next_step = new_conf.next_step();
      if (add(configurations[pos][next_step], new_conf)) {
        add(new_configurations[next_step], new_conf);
        if (add(prev_new_configurations[next_step], new_conf)) {
          change = true;
        }
      }
    }
  }
  return change;
}

void Algo::fit(const Grammar &input_gram) {
  grammar = input_gram;
  std::string first_left = std::string(1, grammar.start);
  grammar.rules['#'].push_back(Rule('#', first_left));
  grammar.nonterminals.insert('#');
  grammar.start = '#';
}

std::string Algo::predict(const std::string &word) {
  configurations.resize(0);
  configurations.resize(word.size() + 1);
  char first_step = grammar.rules[grammar.start][0].second_part[0];
  configurations[0][first_step].push_back(Configuration(grammar.rules[grammar.start][0], Position(0, 0)));
  for (int i = 0; i <= word.size(); ++i) {
    if (i) {
      Scan(word[i - 1], i);
    }
    bool update = true;
    prev_new_configurations = configurations[i];
    while (update) {
      update = false;
      update |= Predict(i);
      update |= Complete(i);
      prev_new_configurations = new_configurations;
      new_configurations.clear();
    }
  }
  for (int i = 0; i < configurations[word.size()][EndOfWord].size(); ++i) {
    if (!configurations[word.size()][EndOfWord][i].pos.external) {
      return "YES";
    }
  }
  return "NO";
}