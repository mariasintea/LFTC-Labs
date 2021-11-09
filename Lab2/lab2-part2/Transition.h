#ifndef LAB2_TRANSITION_H
#define LAB2_TRANSITION_H

#include <string>

using namespace std;

class Transition {
private:
    string value;
    string initialState;
    string finalState;
public:
    Transition(const string &initialState, const string &finalState, const string &value);

    const string &getValue() const;

    const string &getInitialState() const;

    const string &getFinalState() const;
};


#endif //LAB2_TRANSITION_H
