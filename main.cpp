#include <iostream>
#define ARMA_DONT_USE_BLAS
#include <armadillo>
#include <chrono>
#include <vector>
#include <algorithm>
#include <chess-engine.h>


using namespace std;

int main() {
    Board b;
    b.load_default_positions();
    b.print();
}