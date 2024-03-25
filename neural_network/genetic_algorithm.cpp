#include <iostream>
#include "armadillo"

#include "main.h"

using namespace std;
using namespace arma;


void genetic_algorithm::generate_new_population(int size){
    cout << "Generating new population" << endl;
}

double genetic_algorithm::evaluate_score(neural_network nn, int games, bool print){
    double result = 0;
    int winner;
    games = games / 2;
    games *= 2;

    for (int i = 0; i < games / 2; i++){
        game g;
        neural_network opponent = population[rand() % population_size];
        winner = g.play_game(nn, opponent, false);

        if (winner == 1){
            result += 1;
        }
        else if(winner == 2){
            result += 0;
        }
        else{
            result += 0.1;
        }
    }

    for (int i = 0; i < games / 2; i++){
        game g;
        neural_network opponent = population[rand() % population_size];
        winner = g.play_game(nn, opponent, false);

        if (winner == 2){
            result += 1;
        }
        else if(winner == 1){
            result += 0;
        }
        else{
            result += 0.1;
        }
    }



    result /= games;
    return result;

}


vector<neural_network> genetic_algorithm::selection(int size){
    vector<double> scores;
    double res;
    for(int i = 0; i < population_size; i++){
        res = evaluate_score(population[i], 20, false);
        scores.push_back(res);
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

neural_network genetic_algorithm::mutate(neural_network nn, double mutation_rate, double standard_deviation){
    default_random_engine generator;
    normal_distribution<double> distribution(0, standard_deviation);
    for (int i = 0; i < nn.weights.size(); i++){
        for (int j = 0; j < nn.weights[i].n_elem; j++){
            if ((double)rand() / RAND_MAX < mutation_rate){
                (nn.weights[i])(j) += distribution(generator);
            }
        }
    }
    for (int i = 0; i < nn.biases.size(); i++){
        for (int j = 0; j < nn.biases[i].n_elem; j++){
            if ((double)rand() / RAND_MAX < mutation_rate){
                (nn.biases[i])(j) += distribution(generator);
            }
        }
    }


    return nn;
}
neural_network genetic_algorithm::crossover(neural_network nn1, neural_network nn2, double cr_rate) {
    neural_network new_nn;
    for (int i = 0; i < nn1.weights.size(); i++){
        for (int j = 0; j < nn1.weights[i].n_elem; j++){
            if ((double)rand() / RAND_MAX < cr_rate){
                (new_nn.weights[i])(j) = (nn1.weights[i])(j);
            }
            else{
                (new_nn.weights[i])(j) = (nn2.weights[i])(j);
            }
        }
    }

    for (int i = 0; i < nn1.biases.size(); i++){
        for (int j = 0; j < nn1.biases[i].n_elem; j++){
            if ((double)rand() / RAND_MAX < cr_rate){
                (new_nn.biases[i])(j) = (nn1.biases[i])(j);
            }
            else{
                (new_nn.biases[i])(j) = (nn2.biases[i])(j);
            }
        }
    }

    return new_nn;
}

void genetic_algorithm::generate_population(){
    auto selected = selection(10);
    vector<neural_network> new_population;
    while (new_population.size() < population_size) {
        neural_network parent1 = selected[rand() % selected.size()];
        neural_network parent2 = selected[rand() % selected.size()];

        auto child = crossover(parent1, parent2, 0.3);

        child = mutate(child, 0.25, 1);

        new_population.push_back(child);
    }
    population = new_population;
}

void genetic_algorithm::train(int generations){
    cout << "Training started" << endl;
    for (int i = 0; i < generations; i++){
        if (i % 15 == 0){
            old = population;
        }
        cout << "Generation " << i << endl;
        generate_population();
    }
    cout << "Training finished" << endl;
}

neural_network genetic_algorithm::best_nn(){
    double best_score = -100;
    neural_network best_nn;
    for (auto nn : population){
        double score = evaluate_score(nn, 20, false);
        if (score > best_score){
            best_score = score;
            best_nn = nn;
        }
    }
    return best_nn;
}