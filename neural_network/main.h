#ifndef CHESS_AI_MAIN_H
#define CHESS_AI_MAIN_H

#include <iostream>
#include "armadillo"

using namespace std;
using namespace arma;


class neural_network{
public:
    mat weights1 = randu<mat>(9, 9);
    mat bias1 = randu<mat>(9, 1);
    mat weights2 = randu<mat>(9, 9);
    mat bias2 = randu<mat>(9, 1);
    mat weights3 = randu<mat>(9, 9);
    mat bias3 = randu<mat>(9, 1);
    mat weights4 = randu<mat>(9, 9);
    mat bias4 = randu<mat>(9, 1);
    mat weights5 = randu<mat>(1, 9);
    mat bias5 = randu<mat>(1, 1);


    vector<mat> weights = {weights1, weights2, weights3, weights4, weights5};
    vector<mat> biases = {bias1, bias2, bias3, bias4, bias5};

    double forward(cube input);
    mat convolution(mat input, mat weights);
    cube convolution(cube input, cube weights);
    cube zero_borders(cube input, int padding);
    mat pool(mat input, int stride);
    cube pool(cube input, int stride);
    mat tensor_to_matrix(cube input);
    vec flatten(mat input);
    vec fully_connected(vec input, mat weights);
    vec softmax(vec input);
    double sigmoid(double x);
    vec sigmoid(vec x);
    mat sigmoid(mat x);
    string save();
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
    int play_game(neural_network nn1, neural_network nn2, bool print);
    int play_game(neural_network nn);
};

class genetic_algorithm{
public:
    int population_size;
    vector<neural_network> population;
    vector<neural_network> old;

    void generate_new_population(int size);
    double evaluate_score(neural_network nn, int games, bool print);
    vector<neural_network> selection(int size);
    neural_network mutate(neural_network nn, double mutation_rate, double standard_deviation);
    neural_network crossover(neural_network nn1, neural_network nn2, double cr_rate);
    void generate_population();
    void train(int generations);
    neural_network best_nn();
};


#endif //CHESS_AI_MAIN_H
