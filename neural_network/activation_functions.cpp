#include "main.h"

using namespace std;


double neural_network::sigmoid(double x){
    return 1 / (1 + exp(-x));
}

vec neural_network::sigmoid(vec x){
    vec result(x.n_rows);
    for (int i = 0; i < x.n_rows; i++){
        result(i) = sigmoid(x(i));
    }
    return result;
}

mat neural_network::sigmoid(mat x){
    mat result(x.n_rows, x.n_cols);
    for (int i = 0; i < x.n_rows; i++){
        for (int j = 0; j < x.n_cols; j++){
            result(i, j) = sigmoid(x(i, j));
        }
    }
    return result;
}


double neural_network::softmax(double x){
    return exp(x) / sum(exp(x));
}

vec neural_network::softmax(vec input){
    vec result(input.n_rows);
    for (int i = 0; i < input.n_rows; i++){
        result(i) = softmax(input(i));
    }
    return result;
}

mat neural_network::softmax(mat input){
    mat result(input.n_rows, input.n_cols);
    for (int i = 0; i < input.n_rows; i++){
        for (int j = 0; j < input.n_cols; j++){
            result(i, j) = softmax(input(i, j));
        }
    }
    return result;
}

double neural_network::relu(double x){
    return x > 0 ? x : 0;
}

vec neural_network::relu(vec x){
    vec result(x.n_rows);
    for (int i = 0; i < x.n_rows; i++){
        result(i) = relu(x(i));
    }
    return result;
}

mat neural_network::relu(mat x){
    mat result(x.n_rows, x.n_cols);
    for (int i = 0; i < x.n_rows; i++){
        for (int j = 0; j < x.n_cols; j++){
            result(i, j) = relu(x(i, j));
        }
    }
    return result;
}

