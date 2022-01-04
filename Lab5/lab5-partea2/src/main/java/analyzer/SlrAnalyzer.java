package analyzer;

import entity.Grammar;
import javafx.util.Pair;

import java.util.*;

import static java.lang.String.format;
import static util.UtilsPrinter.*;

public class SlrAnalyzer {
    private Grammar grammar;
    private List<Map<String, String>> actions = new ArrayList<>();
    private Map<String, Set<String>> firstSet = new HashMap<>();
    private Map<String, Set<String>> followSet = new HashMap<>();

    public SlrAnalyzer(String grammarPath) throws Exception {
        grammar = new Grammar(grammarPath);
        buildFirst();
        buildFollow();
        buildActions();
        /*printProductions(grammar);
        printFirstAndFollow(firstSet, followSet);
        printStates(grammar.getStatesSet());
        printActionsTable(grammar, actions);*/
    }

    private void buildFirst() throws Exception {
        for (String nonT : grammar.getNonTerminals()) {
            firstSet.put(nonT, grammar.getFirst(nonT, new HashSet<String>()));
        }
    }

    private void buildFollow() throws Exception {
        for (String nonT : grammar.getNonTerminals()) {
            followSet.put(nonT, grammar.getFollow(nonT, new HashSet<String>()));
        }
    }


    private void buildActions() throws Exception {
        for (int step = 0; step < grammar.getStatesSet().size(); step++) {
            actions.add(grammar.getStateById(step).getActions());
        }
    }

    public boolean checkSequence(String sequenceString) throws Exception {
        List<Pair<String, Integer>> stack = new ArrayList<>();
        stack.add(new Pair<>("$", 0));
        List<String> sequence = initializeSequence(sequenceString);
        List<Integer> output = new ArrayList<>();

        boolean isValid = true;
        boolean isNotFinished = true;
        int iterNo = 0;
        while (isValid && isNotFinished) {
            System.out.println(format("Iteration #%d:", iterNo));
            System.out.print("Stack:\t\t\t\t[ ");
            for (Pair pair : stack) {
                System.out.printf("%s%d ", pair.getKey() ,pair.getValue());
            }
            System.out.println("]");
            System.out.print("Sequence:\t\t\t");
            System.out.println(sequence);
            System.out.print("Output:\t\t\t\t");
            System.out.println(output);
            iterNo++;

            Pair<String, Integer> lastPair = stack.get(stack.size() - 1);
            String currentToken = sequence.get(0);
            if (actions.get(lastPair.getValue()).containsKey(currentToken)) {
                String transition = actions.get(lastPair.getValue()).get(currentToken);
                System.out.println(format("Next operation:\t\t[%s]", transition));
                System.out.println();

                if (transition.startsWith("SHIFT")) {
                    Integer nextId = Integer.parseInt(transition.replace(format("%s ", "SHIFT"), ""));
                    sequence.remove(0);
                    Pair<String, Integer> nextPair = new Pair<>(currentToken, nextId);
                    stack.add(nextPair);
                } else if (transition.startsWith("REDUCE")) {
                    Integer nextId = Integer.parseInt(transition.replace(format("%s ", "REDUCE"), ""));
                    List<String> action = grammar.getProductionById(nextId).getProductionElements();
                    for (int step = action.size() - 1; step >= 0; step--) {
                        String currentElem = action.get(step);
                        Pair<String, Integer> currentLastPair = stack.get(stack.size() - 1);
                        if (currentElem.equals(currentLastPair.getKey())) {
                            stack.remove(stack.size() - 1);
                        } else {
                            throw new Exception("Error during parsing!!!");
                        }
                    }
                    stack.add(buildNewPair(stack.get(stack.size() - 1).getValue(), nextId));
                    output.add(0, nextId);
                } else if (transition.equals("ACCEPT")) {
                    isNotFinished = false;
                } else {
                    throw new Exception(format("Invalid transition \"%s\"", transition));
                }
            } else {
                isValid = false;
            }
        }


        boolean isParsed = !isNotFinished;
        return isValid && isParsed;
    }

    private Pair<String, Integer> buildNewPair(Integer lastId, Integer nextId) throws Exception {
        String newToken = grammar.getProductionById(nextId).getOrigin();
        String newStringId = actions.get(lastId).get(newToken);
        Integer newId;
        if (newStringId.startsWith("REDUCE")) {
            newId = Integer.parseInt(newStringId.replace(format("%s ", "REDUCE"),""));
        } else if(newStringId.startsWith("SHIFT")) {
            newId = Integer.parseInt(newStringId.replace(format("%s ", "SHIFT"),""));
        } else {
            throw new Exception("Exception during parse.");
        }
        return new Pair<>(newToken, newId);
    }

    private List<String> initializeSequence(String sequenceString) throws Exception {
        List<String> sequence = new ArrayList<>(Arrays.asList(sequenceString.split(" ")));
        if (sequence.contains("$")) {
            throw new Exception("Invalid element \"$\" found");
        }
        sequence.add("$");
        return sequence;
    }
}
