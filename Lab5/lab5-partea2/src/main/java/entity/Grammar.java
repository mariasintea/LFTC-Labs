package entity;

import util.UtilsReader;

import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class Grammar {
    private Set<Production> productionsSet;
    private Set<State> statesSet = new HashSet<>();

    public Grammar(String filePath) throws Exception {
        productionsSet = UtilsReader.readProductionsFromFile(filePath);
        generateStates();
    }

    public boolean isTerminal(String toCheckString) {
        return !isNonTerminal(toCheckString);
    }

    public Production getProductionById(int id) throws Exception {
        for (Production production : productionsSet) {
            if (production.getProdId() == id) {
                return production;
            }
        }
        throw new Exception("Index out of bounds.");
    }

    public State getStateById(int id) throws Exception {
        for (State state : statesSet) {
            if (state.getStateId() == id) {
                return state;
            }
        }
        throw new Exception("Index out of bounds.");
    }

    public boolean isNonTerminal(String toCheckString) {
        for (Production production : productionsSet) {
            if (production.getOrigin().equals(toCheckString)) {
                return true;
            }
        }
        return false;

    }

    public Set<String> getTerminals() {
        Set<String> terminals = new HashSet<>();
        for (Production production : productionsSet) {
            for (String elem : production.getProductionElements()) {
                if (isTerminal(elem)) {
                    terminals.add(elem);
                }
            }
        }
        return terminals;
    }

    public Set<String> getFirst(String nonTerminal, Set<String> usedTerminals) throws Exception {
        Set<String> firstSet = new HashSet<>();

        addFirstElems(nonTerminal, usedTerminals, firstSet);

        return firstSet;
    }

    private void addFirstElems(String nonTerminal, Set<String> usedTerminals, Set<String> firstSet) throws Exception {
        for (Production production : getProductionsWithOrigin(nonTerminal)) {
            String localFirst = production.getProductionElements().get(0);
            if (isTerminal(localFirst)) {
                firstSet.add(localFirst);
            } else {
                if (!usedTerminals.contains(localFirst)) {
                    usedTerminals.add(localFirst);
                    firstSet.addAll(getFirst(localFirst, usedTerminals));
                }
            }
        }
    }

    private boolean addFollowElems(String nonTerminal, Set<String> followSet, Set<String> usedNonTerminals) throws Exception {
        boolean found = false;
        for (Production production : productionsSet) {
            List<String> actionsElem = production.getProductionElements();
            for (int step = 0; step < actionsElem.size(); step++) {
                String elem = actionsElem.get(step);
                if (elem.equals(nonTerminal)) {
                    found = true;
                    String toAdd;
                    if (step + 1 < actionsElem.size()) {
                        toAdd = actionsElem.get(step + 1);
                    } else {
                        toAdd = production.getOrigin();
                    }
                    if (isTerminal(toAdd)) {
                        followSet.add(toAdd);
                    } else {
                        if (!usedNonTerminals.contains(toAdd)) {

                            if (step + 1 < actionsElem.size()) {
                                followSet.addAll(getFirst(toAdd, new HashSet<String>()));
                            } else {
                                followSet.addAll(getFollow(toAdd, usedNonTerminals));
                            }

                        }
                    }
                }
            }
        }
        return found;
    }

    public Set<String> getFollow(String nonTerminal, Set<String> usedNonTerminals) throws Exception {
        usedNonTerminals.add(nonTerminal);
        Set<String> followSet = new HashSet<>();
        boolean found = addFollowElems(nonTerminal, followSet, usedNonTerminals);
        if (!found) {
            followSet.add("$");
        }
        return followSet;
    }

    public Set<String> getNonTerminals() {
        Set<String> nonTerminals = new HashSet<>();
        for (Production production : productionsSet) {
            nonTerminals.add(production.getOrigin());
        }
        return nonTerminals;
    }

    private void generateStates() throws Exception {
        Set<Integer> visitedIdsSet = new HashSet<>();
        Set<Production> entrySet = new HashSet<>();
        entrySet.add(getProductionById(0));
        State initialState = new State(this, entrySet);
        initialState.setStateId(0);
        statesSet.add(initialState);
        int currentStateId = 1;
        while (visitedIdsSet.size() < statesSet.size()) {
            Set<State> newStates = new HashSet<>();
            for (State state : statesSet) {
                if (!visitedIdsSet.contains(state.getStateId())) {
                    visitedIdsSet.add(state.getStateId());
                    for (Map.Entry<String, Set<Production>> entry : state.getDerivable().entrySet()) {
                        State posibleState = new State(this, entry.getValue());
                        boolean ok = true;
                        for (State s : statesSet) {
                            if (s.equals(posibleState)) {
                                state.putTransition(entry.getKey(), s.getStateId());
                                ok = false;
                            }
                        }
                        if (ok) {
                            for (State s : newStates) {
                                if (s.equals(posibleState)) {
                                    state.putTransition(entry.getKey(), s.getStateId());
                                    ok = false;
                                }
                            }
                        }
                        if (ok) {
                            posibleState.setStateId(currentStateId);
                            state.putTransition(entry.getKey(), currentStateId);
                            currentStateId++;
                            newStates.add(posibleState);
                        }
                    }
                }
            }
            statesSet.addAll(newStates);
        }
    }

    public Set<Production> getProductionsWithOrigin(String origin) {
        Set<Production> resultSet = new HashSet<>();
        for (Production prod : productionsSet) {
            if (prod.getOrigin().equals(origin)) {
                resultSet.add(prod);
            }
        }
        return resultSet;
    }

    public Set<State> getStatesSet() {
        return statesSet;
    }

    public Set<Production> getProductionSet() {
        return productionsSet;
    }
}
