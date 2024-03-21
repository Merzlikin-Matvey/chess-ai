#ifndef CHESS_AI_MAIN_H
#define CHESS_AI_MAIN_H

#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;


class neural_network{
public:
    cube weights1 = randu(2, 2, 2);
    vec weights2 = randu(4);

    double forward(cube input);
    mat convolution(mat input, mat weights);
    cube convolution(cube input, cube weights);
    cube zero_borders(cube input, int padding);
    mat pool(mat input, int stride);
    cube pool(cube input, int stride);
    mat tensor_to_matrix(cube input);
    vec flatten(mat input);
    vec fully_connected(vec input, vec weights);
    double output(vec input);
};

class game{
public:
    vector<vector<int>> board;

    game() : board(3, vector<int>(3, 0)) {}

    void print_board();
    void set(int x, int y, int player_num);
    int winner();
    vector<pair<int, int>> available_moves();
    cube convert_board(const vector<std::vector<int>>& v, int your_team);
    int play_game(neural_network nn1, neural_network nn2);
    int play_game(neural_network nn);
};

class genetic_algorithm{
public:
    int population_size;
    vector<neural_network> population;
    vector<neural_network> old;

    void generate_new_population(int size);
    double evaluate_score(neural_network nn, int games);
    vector<neural_network> selection(int size);
    neural_network mutate(neural_network nn, double mutation_rate);
    void increase_population(vector<neural_network>);
    void train(int generations);
    neural_network best_nn();
};


#endif //CHESS_AI_MAIN_H
