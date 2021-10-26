#ifndef LAB2_AF_H
#define LAB2_AF_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "Transition.h"

using namespace std;

class AF {
private:
    vector<string> alphabet;
    string initialState;
    vector<Transition> transitions;
    vector<string> states;
    vector<string> finalStates;
public:
    // constructor
    AF(const vector<string> &alphabet, const vector<string> &states, const string &initialState,
       const vector<Transition> &transitions, const vector<string> &finalStates);

    // getter pentru alfabet
    const vector<string> &getAlphabet() const;

    // getter pentru tranzitii
    const vector<Transition> &getTransitions() const;

    // getter pentru starile finale
    const vector<string> &getFinalStates() const;

    // getter pentru stari
    const vector<string> &getStates() const;

    // functie care verifica apartenenta unei secvente la AF
    bool checkSequence(string sequence);

    // functie care returneaza cel mai lung prefix al unei secvente acceptat de AF
    string getGreatestPrefix(string sequence);
};


#endif
