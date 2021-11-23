#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Grammar.h"

using namespace std;

void readDataFromFile(vector<string> &productionRules){
    string line;
    ifstream fin("date.txt");

    while(!fin.eof()){
        getline(fin, line);
        productionRules.push_back(line);
    }
}

void printTerminals(Grammar grammar){
    for(auto termianl: grammar.getTerminals())
        cout << termianl << " ";
    cout << "\n";
}

int main() {
    vector<string> productionRules;
    readDataFromFile(productionRules);
    Grammar grammar = Grammar(productionRules);
    printTerminals(grammar);
    return 0;
}
