#include <iostream>
#include <vector>
#include <stack>
#include "Grammar.cpp"
#include "Transition.cpp"
#include "Configuration.cpp"


class Algo {
public:
    Grammar grammar;
    std::vector<std::unordered_map<char, Transition>> table;
    std::vector<std::vector<Configuration>> states;
    const char EndOfWord = '$';

    void merge(std::unordered_set<char> &helper, std::unordered_set<char> &res);

    std::unordered_set<char> make_first(char step, std::unordered_set<char> &used);

    void update_state(std::vector<Configuration> &state, Configuration &conf);

    void closure(std::vector<Configuration> &state);

    void reduce(int pos);

    std::vector<Configuration> go_to(std::vector<Configuration> &state, char step);

    int check_new(std::vector<Configuration> &state);

    void add_state(std::unordered_set<char> &symbols, int pos);

    void make_table();

    void fit_grammar(const Grammar &input_gram);

    void prepare_states();

    Algo() = default;

    void fit(const Grammar &input_gram);

    std::string predict(std::string word);
};
