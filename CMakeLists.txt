cmake_minimum_required(VERSION 3.10)
project(chess-ai)

add_subdirectory(dependencies/conradsnicta/armadillo-code/armadillo-code-unstable)
add_subdirectory(dependencies/Merzlikin-Matvey/chess/chess-main)

add_executable(Chess_AI main.cpp
        src/neural_network.cpp
        src/tic_tac_toe.cpp
        src/genetic_algorithm.cpp
        src/main.h
        src/activation_functions.cpp
        src/import_export.cpp)

target_link_libraries(Chess_AI armadillo)
target_link_libraries(Chess_AI chess-engine)

target_include_directories(Chess_AI PRIVATE dependencies/Merzlikin-Matvey/chess/chess-main/src)
include_directories(${CMAKE_SOURCE_DIR}/cmake-build-debug/dependencies/Merzlikin-Matvey/chess/chess-main)

set(BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build")
file(MAKE_DIRECTORY ${BUILD_DIRECTORY})

set(LIST
        ${CMAKE_SOURCE_DIR}/settings.json
        ${CMAKE_BINARY_DIR}/dependencies/conradsnicta/armadillo-code/armadillo-code-unstable/libarmadillo.dll
        ${CMAKE_BINARY_DIR}/dependencies/Merzlikin-Matvey/chess/chess-main/libchess-engine.dll
        ${CMAKE_BINARY_DIR}/dependencies/Merzlikin-Matvey/chess/chess-main/default_positions.json
)

add_custom_command(TARGET Chess_AI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/settings.json
        ${BUILD_DIRECTORY})

foreach(PATH ${LIST})
    add_custom_command(TARGET Chess_AI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PATH}
            ${BUILD_DIRECTORY})
endforeach()

add_custom_command(TARGET Chess_AI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Chess_AI> ${BUILD_DIRECTORY})

set_target_properties(Chess_AI PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIRECTORY})