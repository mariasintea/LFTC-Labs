#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "AF.h"

using namespace std;

const string fileName = "constante.txt";

/**
 * imparte linia dupa spatii si adauga in vectorul dat v
 * @param v - vector dat
 * @param line - linia data
 */
void splitAndAdd(vector<string> &v, string line){
    string  word = line.substr(0, line.find(" "));
    while (!line.empty()) {
        v.push_back(word);
        line.erase(0, word.length() + 1);
        word = line.substr(0, line.find(" "));
    }
}

/**
 * citeste datele din fisier
 * @param alphabet - lista in care va fi adaugat alfabetul dat
 * @param states - lista in care vor fi adaugate starile dat
 * @param initialState - lista in care va fi adaugata starea initiala dat
 * @param transitions - lista in care vor fi adaugate tranzitiile dat
 * @param finalStates - lista in care vor fi adaugate starile finale dat
 */
void readDataFromFile(vector<string> &alphabet, vector<string> &states, string &initialState,
                      vector<Transition> &transitions, vector<string> &finalStates){
    string line;
    int n;
    ifstream fin(fileName);

    getline(fin, line);
    splitAndAdd(alphabet, line);

    getline(fin, line);
    splitAndAdd(states, line);

    fin >> initialState;
    fin.get();

    getline(fin, line);
    splitAndAdd(finalStates, line);

    while(!fin.eof()){
        getline(fin, line);
        string  initialState = line.substr(0, line.find(" "));
        line.erase(0, initialState.length() + 2);
        string  value = line.substr(0, line.find("-> "));
        line.erase(0, initialState.length() + 2);
        string  finalState = line;

        transitions.push_back(Transition(initialState, finalState, value));
    }
}

/**
 * citeste datele de la tastatura
 * @param alphabet - lista in care va fi adaugat alfabetul dat
 * @param states - lista in care vor fi adaugate starile dat
 * @param initialState - lista in care va fi adaugata starea initiala dat
 * @param transitions - lista in care vor fi adaugate tranzitiile dat
 * @param finalStates - lista in care vor fi adaugate starile finale dat
 */
void readDataFromUser(vector<string> &alphabet, vector<string> &states, string &initialState,
                      vector<Transition> &transitions, vector<string> &finalStates){
    string line;
    int n;

    cout << "Dati alfabetul starilor(string string ... string): ";
    cin >> line;
    splitAndAdd(alphabet, line);

    cout << "Dati multimea starilor(string string ... string): ";
    cin >> line;
    splitAndAdd(states, line);

    cout << "Dati starea initiala(string): ";
    cin >> initialState;

    cout << "Dati multimea starilor finale: ";
    cin >> line;
    splitAndAdd(finalStates, line);

    cout << "Dati numarul de tranzitii(int): ";
    cin >> n;
    cout << "Dati tranzitiile(forma unei linii: string -string-> string): ";
    for (int i = 0; i < n; i++){
        cin >> line;
        string  initialState = line.substr(0, line.find(" "));
        line.erase(0, initialState.length() + 2);
        string  finalState = line.substr(0, line.find("-> "));
        line.erase(0, initialState.length() + 3);
        string  value = line;

        transitions.push_back(Transition(initialState, finalState, value));
    }
}

/**
 * afiseaza pe ecran starile pentru un AF dat
 * @param finiteStateAutomaton - AF dat
 */
void printStates(AF finiteStateAutomaton){
    for(auto state: finiteStateAutomaton.getStates())
        cout << state << " ";
    cout << "\n";
}

/**
 * afiseaza pe ecran alfabetul pentru un AF dat
 * @param finiteStateAutomaton - AF dat
 */
void printAlphabet(AF finiteStateAutomaton){
    for(auto letter: finiteStateAutomaton.getAlphabet())
        cout << letter << " ";
    cout << "\n";
}

/**
 * afiseaza pe ecran tranzitiile pentru un AF dat
 * @param finiteStateAutomaton - AF dat
 */
void printTransitions(AF finiteStateAutomaton) {
    for(auto transition: finiteStateAutomaton.getTransitions())
        cout << transition.getInitialState() << " -" << transition.getValue() << "-> "  <<
            transition.getFinalState() << "\n";
    cout << "\n";
}

/**
 * afiseaza pe ecran starile finale pentru un AF dat
 * @param finiteStateAutomaton - AF dat
 */
void printFinalStates(AF finiteStateAutomaton) {
    for(auto state: finiteStateAutomaton.getFinalStates())
        cout << state << " ";
    cout << "\n";
}

/**
 * afiseaza pe ecran daca o secventa citita de la tastatura este acceptata de AF
 * @param finiteStateAutomaton - AF dat
 */
void checkSequence(AF finiteStateAutomaton) {
    string line;
    cout << "Dati o secventa:";
    cin >> line;
    if (finiteStateAutomaton.checkSequence(line))
        cout << "Este acceptata de AF!\n";
    else
        cout << "Nu este acceptata de AF!\n";
}

/**
 * afiseaza pe ecran cel mai lung prefix acceptat de AF pentru o secventa citita de la tastatura
 * @param finiteStateAutomaton - AF dat
 */
void printGreatestPrefix(AF finiteStateAutomaton) {
    string line;
    cout << "Dati o secventa:";
    cin >> line;
    string rez = finiteStateAutomaton.getGreatestPrefix(line) ;
    if (rez == "")
        cout << "Nu exista vreo subsecventa acceptata!\n";
    else
        cout << rez << "\n";
}

/**
 * afiseaza pe ecran meniul pentru modul in care se va face citirea
 */
void printReadingMenu(){
    cout << "1 - Citire date din fisier\n";
    cout << "2 - Citire date de la tastatura\n";
}

/**
 * afiseaza pe ecran meniul principal
 */
void printMainMenu(){
    cout << "Afisare:\n";
    cout << "1 - Multimea starilor\n";
    cout << "2 - Alfabetul\n";
    cout << "3 - Tranzitiile\n";
    cout << "4 - Multimea starilor finale\n";
    cout << "5 - Verificare daca o secventa este acceptata de AF\n";
    cout << "6 - Cel mai lung prefix al unei secvente care este acceptat de AF\n";
}

int main() {
    int command;
    vector<string> alphabet;
    vector<string> states;
    string initialState;
    vector<Transition> transitions;
    vector<string> finalStates;

    // citirea datelor
    printReadingMenu();
    cout << "Introduceti comanda:";
    cin >> command;
    switch (command){
        case 1: readDataFromFile(alphabet, states, initialState, transitions, finalStates); break;
        case 2: readDataFromUser(alphabet, states, initialState, transitions, finalStates); break;
        default: {
            cout << "Comanda invalida!";
            return 0;
        }
    }

    AF finiteStateAutomaton = AF(alphabet, states, initialState, transitions, finalStates);

    // meniul principal
    while(true){
        printMainMenu();
        cout << "Introduceti comanda:";
        cin >> command;
        switch (command){
            case 1: printStates(finiteStateAutomaton); break;
            case 2: printAlphabet(finiteStateAutomaton); break;
            case 3: printTransitions(finiteStateAutomaton); break;
            case 4: printFinalStates(finiteStateAutomaton); break;
            case 5: checkSequence(finiteStateAutomaton); break;
            case 6: printGreatestPrefix(finiteStateAutomaton); break;
            default: {
                cout << "Comanda invalida!";
                return 0;
            }
        }
    }
    return 0;
}
