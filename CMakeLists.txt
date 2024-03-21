cmake_minimum_required(VERSION 3.10)
project(chess-ai)

add_subdirectory(neural_network)
add_subdirectory(dependencies/conradsnicta/armadillo-code/armadillo-code-unstable)

add_executable(Chess_AI main.cpp)

target_link_libraries(Chess_AI Neural_Network armadillo)
