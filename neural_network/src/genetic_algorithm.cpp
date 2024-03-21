#include <iostream>
#include <armadillo>

#include "main.h"

using namespace std;
using namespace arma;


void genetic_algorithm::generate_new_population(int size){
    population_size = size;
    for(int i = 0; i < size; i++){
        neural_network nn;
        population.push_back(nn);
    }
    old = population;
}

double genetic_algorithm::evaluate_score(neural_network nn, int games){
    double result;
    for (int i = 0; i < games; i++){
        game g;
        neural_network opponent = old[rand() % population_size];
        int player = 1;
        while (g.winner() == 0){
            auto moves = g.available_moves();
            vector<double> scores;
            for (auto move : moves){
                auto new_board = g;
                new_board.set(move.first, move.second, player);
                auto input = g.convert_board(new_board.board, player);
                double score;
                if (player == 1){
                    score = nn.forward(input);
                    player = 2;
                }
                else{
                    score = opponent.forward(input);
                    player = 1;
                }
                scores.push_back(score);
            }
            int best_move = 0;
            for (int i = 0; i < scores.size(); i++){
                if (scores[i] > scores[best_move]){
                    best_move = i;
                }
            }
            g.set(moves[best_move].first, moves[best_move].second, player);
        }
        if (g.winner() == 1){
            result += 1;
        }
        else if(g.winner() == 2){
            result -= 1;
        }
        else{
            result += 0.3;
        }
    }
    result /= games;
    return result;

}


vector<neural_network> genetic_algorithm::selection(int size){
    vector<double> scores;
    for(int i = 0; i < population_size; i++){
        scores.push_back(evaluate_score(population[i], 10));
    }

    vector<neural_network> new_population;
    for(int i = 0; i < size; i++){
        double sum = 0;
        for(auto score : scores){
            sum += score;
        }
        double random = (double)rand() / RAND_MAX * sum;
        double current_sum = 0;
        for(int j = 0; j < population_size; j++){
            current_sum += scores[j];
            if (current_sum > random){
                new_population.push_back(population[j]);
                break;
            }
        }
    }
    return new_population;
}

neural_network genetic_algorithm::mutate(neural_network nn, double mutation_rate){
    default_random_engine generator;
    normal_distribution<double> distribution(0, 0.15);
    for (int i = 0; i < nn.weights1.n_elem; i++){
        if ((double)rand() / RAND_MAX < mutation_rate){
            nn.weights1(i) += distribution(generator);
        }
    }
    for (int i = 0; i < nn.weights2.n_elem; i++){
        if ((double)rand() / RAND_MAX < mutation_rate){
            nn.weights2(i) += distribution(generator);
        }
    }
    return nn;
}

void genetic_algorithm::increase_population(vector<neural_network> new_population){
    population = new_population;
    int i = 0;
    while (population.size() < population_size){
        population.push_back(mutate(new_population[i % new_population.size()], 0.25));
    }
}

void genetic_algorithm::train(int generations){
    cout << "Training started" << endl;
    for (int i = 0; i < generations; i++){
        if (i % 10 == 5){
            old = population;
        }
        cout << "Generation " << i << endl;
        vector<neural_network> selected = selection(20);
        increase_population(selected);
    }
    cout << "Training finished" << endl;
}

neural_network genetic_algorithm::best_nn(){
    double best_score = -100;
    neural_network best_nn;
    for (auto nn : population){
        double score = evaluate_score(nn, 50);
        if (score > best_score){
            best_score = score;
            best_nn = nn;
        }
    }
    return best_nn;
}