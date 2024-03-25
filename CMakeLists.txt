cmake_minimum_required(VERSION 3.10)
project(chess-ai)

add_subdirectory(neural_network)
add_subdirectory(dependencies/conradsnicta/armadillo-code/armadillo-code-unstable)

add_executable(Chess_AI main.cpp)

target_link_libraries(Chess_AI Neural_Network armadillo)

add_custom_command(TARGET Chess_AI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/settings.json
        $<TARGET_FILE_DIR:Chess_AI>)