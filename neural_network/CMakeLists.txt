cmake_minimum_required(VERSION 3.10)
project(neural_network)

add_library(Neural_Network src/neural_network.cpp
        src/tic_tac_toe.cpp
        src/genetic_algorithm.cpp
        src/main.h
        activation_functions.cpp
        src/import_export.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/conradsnicta/armadillo-code/armadillo-code-unstable/include)