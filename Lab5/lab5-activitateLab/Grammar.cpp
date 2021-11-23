#include "Grammar.h"

Grammar::Grammar(const vector<string> &productionRules)
        : productionRules(productionRules) {}

set<char> Grammar::getTerminals() {
    for(auto rule: productionRules){
        for(auto character: rule){
            if (character >= 'a' && character <= 'z')
                terminals.insert(character);
        }
    }
    return terminals;
}
