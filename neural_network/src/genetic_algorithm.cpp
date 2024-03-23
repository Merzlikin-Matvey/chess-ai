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

double genetic_algorithm::evaluate_score(neural_network nn, int games, bool print){
    double result = 0;
    int winner;
    games = games / 2;
    games *= 2;

    if (print){
        cout << "First half of games: " << endl;
    }
    for (int i = 0; i < games / 2; i++){
        game g;
        neural_network opponent = old[rand() % population_size];
        winner = g.play_game(nn, opponent, print);

        if (winner == 1){
            result += 1;
        }
        else if(winner == 2){
            result -= 1;
        }
        else{
            result += 0.1;
        }
    }
    if (print){
        cout << "Second half of games: " << endl;
    }

    for (int i = 0; i < games / 2; i++){
        game g;
        neural_network opponent = old[rand() % population_size];
        winner = g.play_game(nn, opponent, print);

        if (winner == 2){
            result += 1;
        }
        else if(winner == 1){
            result -= 1;
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
    for(int i = 0; i < population_size; i++){
        scores.push_back(evaluate_score(population[i], 10, false));
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

neural_network genetic_algorithm::crossover(neural_network nn1, neural_network nn2, double cr_rate) {
    neural_network new_nn;
    for (int i = 0; i < nn1.weights1.n_elem; i++){
        if ((double)rand() / RAND_MAX < cr_rate){
            new_nn.weights1(i) = nn1.weights1(i);
        }
        else{
            new_nn.weights1(i) = nn2.weights1(i);
        }
    }
    for (int i = 0; i < nn1.weights2.n_elem; i++){
        if ((double)rand() / RAND_MAX < cr_rate){
            new_nn.weights2(i) = nn1.weights2(i);
        }
        else{
            new_nn.weights2(i) = nn2.weights2(i);
        }
    }
    return new_nn;
}

void genetic_algorithm::generate_population(){
    auto selected = selection(20);
    vector<neural_network> new_population;
    while (new_population.size() < population_size) {
        neural_network parent1 = selected[rand() % selected.size()];
        neural_network parent2 = selected[rand() % selected.size()];

        auto child = crossover(parent1, parent2, 0.4);

        child = mutate(parent1, 0.3, 0.5);

        new_population.push_back(child);
    }
    population = new_population;
}

void genetic_algorithm::train(int generations){
    cout << "Training started" << endl;
    for (int i = 0; i < generations; i++){
        if (i % 10 == 5){
            cout << evaluate_score(best_nn(), 5, true) << endl;
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