#include <iostream>
#include <vector>
#include <algorithm>
#define ARMA_DONT_USE_BLAS
#include <armadillo>

#include "main.h"


using namespace std;

void game::print_board(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void game::set(int x, int y, int player_num){
    board[x][y] = player_num;
}

vector<pair<int, int>> game::available_moves(){
    vector<pair<int, int>> result;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j] == 0){
                result.push_back({i, j});
            }
        }
    }
    return result;
}

int game::winner(){
    for (int i = 0; i < 3; i++){
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0){
            return board[i][0];
        }
    }

    for (int i = 0; i < 3; i++){
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0){
            return board[0][i];
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0){
        return board[0][0];
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0){
        return board[0][2];
    }

    if (this->available_moves().size() == 0){
        return 3;
    }

    return 0;
}

cube game::convert_board(const vector<std::vector<int>>& v, int your_team) {
    cube result(3, 3, 2);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (v[i][j] == 0){
                result(i, j, 0) = 0;
                result(i, j, 1) = 0;
            }
            else if (v[i][j] == your_team){
                result(i, j, 0) = 1;
                result(i, j, 1) = 0;
            }
            else{
                result(i, j, 0) = 0;
                result(i, j, 1) = 1;
            }
        }
    }
    return result;
}

int game::play_game(neural_network nn1, neural_network nn2){
    game g;
    int player = 1;
    while (g.winner() == 0){
        auto moves = g.available_moves();
        vector<double> scores;
        for (auto move : moves){
            auto new_board = g;
            new_board.set(move.first, move.second, player);
            auto input = g.convert_board(new_board.board, player);
            double score;
            if (player == 1){
                score = nn1.forward(input);
                player = 2;
            }
            else{
                score = nn2.forward(input);
                player = 1;
            }
            scores.push_back(score);
        }
        int best_move = 0;
        for (int i = 0; i < scores.size(); i++){
            if (scores[i] > scores[best_move]){
                best_move = i;
            }
        }
        g.set(moves[best_move].first, moves[best_move].second, player);
    }
    return g.winner();
}

int game::play_game(neural_network nn) {
    game g;
    int player = 1;
    int x, y;
    while (g.winner() == 0){
        g.print_board();
        auto moves = g.available_moves();
        vector<double> scores;
        if (player == 1) {
            for (auto move: moves) {
                auto new_board = g;
                new_board.set(move.first, move.second, player);
                auto input = g.convert_board(new_board.board, player);
                double score;
                score = nn.forward(input);
                scores.push_back(score);
            }

            int best_move = 0;
            for (int i = 0; i < scores.size(); i++) {
                if (scores[i] > scores[best_move]) {
                    best_move = i;
                }
            }
            g.set(moves[best_move].first, moves[best_move].second, player);
            player = 2;
        }
        else{
            cout << "Enter x and y: ";
            cin >> x >> y;
            g.set(x, y, player);
            player = 1;
        }
    }
    return g.winner();
}