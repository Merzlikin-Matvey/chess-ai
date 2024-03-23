#include <iostream>
#define ARMA_DONT_USE_BLAS
#include <armadillo>
#include <chrono>
#include <vector>
#include <algorithm>

#include "neural_network/src/main.h"


using namespace std;

int main() {
    neural_network nn;
    genetic_algorithm ga;
    ga.generate_new_population(200);
    ga.train(100);
    int wins = 0;
    int losses = 0;
    int draws = 0;
    int res;

    auto best = ga.selection(30);
    for (int i = 0; i < 10; i++){
        game g;
        res = g.play_game(best[i], nn, false);
        if (res == 1){
            wins++;
        }
        else if (res == 2){
            losses++;
        }
        else{
            draws++;
        }
    }
    cout << "Wins: " << wins << endl;
    cout << "Losses: " << losses << endl;
    cout << "Draws: " << draws << endl;


    for (int i = 0; i < 10; i++){
        game g;
        res = g.play_game(best[i]);
        cout << res << endl;
    }
}