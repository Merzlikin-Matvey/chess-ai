#define ARMA_DONT_USE_BLAS
#include <armadillo>
#include <cmath>

#include "main.h"

using namespace std;
using namespace arma;


double neural_network::forward(cube input){
    mat layer1 = input.slice(0) - input.slice(1);
    vec layer2 = fully_connected(flatten(layer1), weights1);
    double output = sigmoid(as_scalar(layer2));
    return output;
}

mat neural_network::convolution(mat input, mat weights){
    mat result(input.n_rows - weights.n_rows + 1, input.n_cols - weights.n_cols + 1);
    for (int i = 0; i < result.n_rows; i++){
        for (int j = 0; j < result.n_cols; j++){
            result(i, j) = accu(input.submat(i, j, i + weights.n_rows - 1, j + weights.n_cols - 1) % weights);
        }
    }
    return result;
}

cube neural_network::convolution(cube input, cube weights){
    cube result(input.n_rows - weights.n_rows + 1, input.n_cols - weights.n_cols + 1, weights.n_slices);
    for (int i = 0; i < input.n_slices; i++){
        result.slice(i) = convolution(input.slice(i), weights.slice(i));
    }
    return result;
}

cube neural_network::zero_borders(cube input, int padding){
    cube result(input.n_rows + 2 * padding, input.n_cols + 2 * padding, input.n_slices);
    result.subcube(padding, padding, 0, result.n_rows - padding - 1, result.n_cols - padding - 1, result.n_slices - 1) = input;
    return result;
}


mat neural_network::tensor_to_matrix(cube input) {
    mat output = zeros<arma::mat>(input.n_rows, input.n_cols);
    for (uword k = 0; k < input.n_slices; ++k) {
        output += input.slice(k);
    }

    return output;
}

mat neural_network::pool(mat input, int stride){
    mat result(input.n_rows / stride, input.n_cols / stride);
    for (int i = 0; i < result.n_rows; i++){
        for (int j = 0; j < result.n_cols; j++){
            mat sub_mat = arma::conv_to<arma::mat>::from(input.submat(i * stride, j * stride, i * stride + stride - 1, j * stride + stride - 1));
            result(i, j) = sub_mat.max();
        }
    }
    return result;
}

cube neural_network::pool(cube input, int stride){
    cube result(input.n_rows / stride, input.n_cols / stride, input.n_slices);
    for (int i = 0; i < input.n_slices; i++){
        result.slice(i) = pool(input.slice(i), stride);
    }
    return result;
}

vec neural_network::flatten(mat input){
    vec result(input.n_rows * input.n_cols);
    for (int i = 0; i < input.n_rows; i++){
        for (int j = 0; j < input.n_cols; j++){
            result(i * input.n_rows + j) = input(i, j);
        }
    }
    return result;
}

vec neural_network::fully_connected(vec input, vec weights){
    return input.t() * weights;
}


vec neural_network::softmax(vec input){
    vec exp_input = exp(input);
    return exp_input / accu(exp_input);
}

double neural_network::sigmoid(double x){
    return 1 / (1 + exp(-x));
}

cube neural_network::relu(cube input){
    cube output = input;
    output.transform([](double val) { return val > 0.0 ? val : 0.0; });
    return output;
}