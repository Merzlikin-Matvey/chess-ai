#include <iostream>
#define ARMA_DONT_USE_BLAS
#include <armadillo>
#include <chrono>
#include <vector>
#include <algorithm>

#include "neural_network/src/main.h"


using namespace std;

int main() {
    game g;
    neural_network nn;
    g.print_board();
}