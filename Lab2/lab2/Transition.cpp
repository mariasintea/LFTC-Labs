//
// Created by Maria on 21/10/2021.
//

#include "Transition.h"

Transition::Transition(const string &initialState, const string &finalState, const string &value) : value(value),
                                                                                                    initialState(
                                                                                                            initialState),
                                                                                                    finalState(
                                                                                                            finalState) {}

const string &Transition::getValue() const {
    return value;
}

const string &Transition::getInitialState() const {
    return initialState;
}

const string &Transition::getFinalState() const {
    return finalState;
}
