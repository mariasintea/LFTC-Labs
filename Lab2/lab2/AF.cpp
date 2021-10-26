#include "AF.h"

AF::AF(const vector<string> &alphabet, const vector<string> &states, const string &initialState,
       const vector<Transition> &transitions, const vector<string> &finalStates) : alphabet(alphabet),
                                                                               states(states),
                                                                               initialState(initialState),
                                                                               transitions(transitions),
                                                                               finalStates(finalStates) {}

const vector<string> &AF::getAlphabet() const {
    return alphabet;
}

const vector<Transition> &AF::getTransitions() const {
    return transitions;
}

const vector<string> &AF::getFinalStates() const {
    return finalStates;
}

const vector<string> &AF::getStates() const {
    return states;
}

bool AF::checkSequence(string sequence){
    return getGreatestPrefix(sequence) == sequence;
}

string AF::getGreatestPrefix(string sequence){
    string prefix = "";
    string prefixWithFinalState = "";
    string currentState = initialState;

    for (auto letter: sequence) {
        bool ok = false;
        for (auto transaction: transitions){
            if (transaction.getInitialState() == currentState && transaction.getValue().compare(sequence.substr(0, transaction.getValue().length())) == 0){
                prefix += transaction.getValue();
                sequence.erase(0, transaction.getValue().length());
                currentState = transaction.getFinalState();

                for(auto transaction: transitions)
                    if(find(finalStates.begin(), finalStates.end(), currentState) != finalStates.end())
                        prefixWithFinalState = prefix;

                if (sequence.empty())
                    return prefixWithFinalState;
                ok = true;
            }
        }

        if (!ok)
            return prefixWithFinalState;
    }
}