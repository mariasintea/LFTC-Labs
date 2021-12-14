package entity;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import static java.lang.String.format;

public class State {
    private Set<Production> prodDerives = new HashSet<>();
    private Grammar grammar;
    private Map<String, Integer> transitions = new HashMap<>();
    private int stateId = -1;

    public State(Grammar grammar, Set<Production> entrySet) throws Exception {
        this.grammar = grammar;
        derivateProductions(entrySet);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        State state = (State) o;

        return prodDerives.equals(state.prodDerives);

    }

    @Override
    public int hashCode() {
        return prodDerives.hashCode();
    }

    private void derivateProductions(Set<Production> entrySet) throws Exception {
        for (Production production : entrySet) {
            Production derivate = production.derivate();
            if (!prodDerives.contains(derivate)) {
                prodDerives.add(derivate);
                if (derivate.canDerivate()) {
                    derivateProductions(grammar.getProductionsWithOrigin(derivate.getCurrentElem()));
                }
            }
        }
    }

    @Override
    public String toString() {
        String result = format("\nState #%d\n", stateId);
        for (Production production : prodDerives) {
            result += production.toString() + '\n';
        }

        return result;
    }

    public int getStateId() {
        return stateId;
    }

    public void setStateId(int stateId) {
        this.stateId = stateId;
    }

    public void putTransition(String key, int value) throws Exception {
        if (transitions.containsKey(key)) {
            throw new Exception("Duplicate transition");
        }
        transitions.put(key, value);
    }

    public Map<String, Set<Production>> getDerivable() {
        Map<String, Set<Production>> result = new HashMap<>();
        for (Production production : prodDerives) {
            if (production.canDerivate()) {
                String curElem = production.getCurrentElem();
                if (!result.containsKey(curElem)) {
                    result.put(curElem, new HashSet<Production>());
                }
                result.get(curElem).add(production);
            }
        }
        return result;
    }

    public Map<String, Integer> getTransitions() {
        return transitions;
    }

    public Map<String, String> getActions() throws Exception {
        Map<String, String> stateActions = new HashMap<>();
        for (Production production : prodDerives) {
            if (production.canDerivate()) {
                String transKey = production.getCurrentElem();
                Integer transValue = transitions.get(transKey);
                if (stateActions.containsKey(transKey) && !stateActions.get(transKey).equals(format("%s %d", "SHIFT", transValue))) {
                    throw new Exception(format("Action conflict for transition: %s.", transKey));
                }
                stateActions.put(transKey, format("%s %d", "SHIFT", transValue));
            } else {
                for (String prediction : grammar.getFollow(production.getOrigin(), new HashSet<String>())) {
                    Integer transValue = production.getProdId();

                    if (transValue == 0 && prediction.equals("$")) {
                        if (stateActions.containsKey(prediction) && !stateActions.get(prediction).equals(format("%s %d", "ACCEPT", transValue))) {
                            throw new Exception(format("Action conflict for transition: %s.", prediction));
                        }
                        stateActions.put(prediction, format("%s", "ACCEPT"));
                    } else {
                        if (stateActions.containsKey(prediction) && !stateActions.get(prediction).equals(format("%s %d", "REDUCE", transValue))) {
                            throw new Exception(format("Action conflict for transition: %s.", prediction));
                        }
                        stateActions.put(prediction, format("%s %d", "REDUCE", transValue));
                    }
                }
            }
        }
        return stateActions;
    }

    public Set<Production> getProdDerives() {
        return prodDerives;
    }
}
