#pragma once

#include "Grammar.cpp"
#include "Configuration.cpp"

class Algo {
private:
    Grammar grammar;
    const char EndOfWord = '\0';

    bool add(std::vector<Configuration> &confs, Configuration &new_conf);

public:
    std::unordered_map<char, std::vector<Configuration>> prev_new_configurations;

    std::unordered_map<char, std::vector<Configuration>> new_configurations;

    std::vector<std::unordered_map<char, std::vector<Configuration>>> configurations;

    Algo() = default;

    void Scan(char next_symbol, int pos);

    bool Predict(int pos);

    bool Complete(int pos);

    void fit(const Grammar &input_grammar);

    std::string predict(const std::string &word);
};