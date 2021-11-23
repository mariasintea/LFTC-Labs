#ifndef LAB5_ACTIVITATELAB_GRAMMAR_H
#define LAB5_ACTIVITATELAB_GRAMMAR_H

#include <vector>
#include <set>
#include <string>

using namespace std;

class Grammar {
private:
    set<char> terminals;
    set<char> non_terminals;
    vector<string> productionRules;
public:
    Grammar(const vector<string> &productionRules);
    set<char> getTerminals();
};


#endif
