cmake_minimum_required(VERSION 3.10)
project(neural_network)

add_library(Neural_Network neural_network.cpp
        tic_tac_toe.cpp
        genetic_algorithm.cpp
        main.h
)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/conradsnicta/armadillo-code/armadillo-code-unstable/include)