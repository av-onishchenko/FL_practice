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
  int sz = configurations[pos].size();
  for (auto it = configurations[pos].begin(); it != configurations[pos].end(); ++it) {
    char next_nonterm = (*it).first;
    if (!curr_gram.nonterminals.count(next_nonterm)) { continue; };
    for (int k = 0; k < curr_gram.rules[next_nonterm].size(); ++k) {
      char next_step = curr_gram.rules[next_nonterm][k].second_part[0];
      Configuration new_conf = Configuration(curr_gram.rules[next_nonterm][k], Position(pos, 0));
      add(configurations[pos][next_step], new_conf);
    }
  }
  return sz != configurations[pos].size();
}

bool Algo::Complete(int pos) {
  bool change = false;
  for (int i = 0; i < configurations[pos]['\0'].size(); ++i) {
    int prev_pos = configurations[pos]['\0'][i].pos.external;
    char prev_step = configurations[pos]['\0'][i].rule.first_part;
    for (int j = 0; j < configurations[prev_pos][prev_step].size(); ++j) {
      Configuration new_conf = configurations[prev_pos][prev_step][j];
      ++new_conf.pos.interior;
      char next_step = new_conf.next_step();
      if (add(configurations[pos][next_step], new_conf)) {
        change = true;
      }
    }
  }
  return change;
}

void Algo::fit(const Grammar &input_gram) {
  curr_gram = input_gram;
  std::string first_left = std::string(1, curr_gram.start);
  curr_gram.rules['#'].push_back(Rule('#', first_left));
  curr_gram.nonterminals.insert('#');
  curr_gram.start = '#';
}

std::string Algo::predict(const std::string &word) {
  configurations.resize(0);
  configurations.resize(word.size() + 1);
  char first_step = curr_gram.rules[curr_gram.start][0].second_part[0];
  configurations[0][first_step].push_back(Configuration(curr_gram.rules[curr_gram.start][0], Position(0, 0)));
  for (int i = 0; i <= word.size(); ++i) {
    if (i) {
      Scan(word[i - 1], i);
    }
    bool update = true;
    while (update) {
      update = false;
      update |= Predict(i);
      update |= Complete(i);
    }
  }
  for (int i = 0; i < configurations[word.size()]['\0'].size(); ++i) {
    if (!configurations[word.size()]['\0'][i].pos.external) {
      return "YES";
    }
  }
  return "NO";
}