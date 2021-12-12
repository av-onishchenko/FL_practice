#pragma once
#include "Grammar.cpp"
#include "Configuration.cpp"

class Algo {
private:
    Grammar curr_gram;

    bool add(std::vector<Configuration> &confs, Configuration &new_conf);


public:
    std::vector<std::unordered_map<char, std::vector<Configuration>>> configurations;

    Algo() = default;

    void Scan(char next_symbol, int pos);

    bool Predict(int pos);

    bool Complete(int pos);

    void fit(const Grammar &input_gram);

    std::string predict(const std::string &word);
};