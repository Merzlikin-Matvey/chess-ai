#include "main.h"

using namespace std;


string neural_network::save(string path){
    ofstream file;
    file.open(path);
    file << "preamble_start" << endl;
    time_t now = time(0);
    char* dt = ctime(&now);
    file << dt << endl;
    file << weights.size() << " " << biases.size() << endl;
    file << "preamble_end" << endl;
    for (int i = 0; i < weights.size(); i++){
        file << weights[i] << endl;
    }
    for (int i = 0; i < biases.size(); i++){
        file << biases[i] << endl;
    }
    file.close();
    return path;
}