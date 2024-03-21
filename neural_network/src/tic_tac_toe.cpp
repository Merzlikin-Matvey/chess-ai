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
    cout << 1 << endl;
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

    return -1;
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

