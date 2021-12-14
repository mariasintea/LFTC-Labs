package util;

import entity.Grammar;
import entity.Production;
import entity.State;

import java.util.*;

import static java.lang.Math.max;
import static java.lang.String.format;

public class UtilsPrinter {
    public  static void printActionsTable(Grammar grammar, List<Map<String, String>> actions) throws Exception {
        System.out.println("Action table:");
        Set<String> nonTerminals = grammar.getNonTerminals();
        nonTerminals.remove(grammar.getProductionById(0).getOrigin());
        Set<String> terminals = grammar.getTerminals();
        terminals.add("$");
        List<Integer> columnLength = new ArrayList<>();
        computeColumnLengths(nonTerminals, terminals, columnLength, grammar);
        String separator = getLineSeparator(columnLength);
        printTableHeader(nonTerminals, terminals, columnLength, separator);
        printTableContent(nonTerminals, terminals, columnLength, separator, actions);
        System.out.println();
    }

    private static void printTableContent(Set<String> nonTerminals, Set<String> terminals, List<Integer> columnLength, String separator, List<Map<String, String>> actions) {
        for (int step = 0; step < actions.size(); step++) {
            System.out.print("|");
            for (int i = 0; i < (columnLength.get(0) - String.valueOf(step).length())/2; i++) {
                System.out.print(" ");
            }
            System.out.printf("I%d", step);
            for (int i = 0; i < (columnLength.get(0) - String.valueOf(step).length() + 1) / 2 + 1; i++) {
                System.out.print(" ");
            }
            System.out.print("|");
            int currentColumn = 1;
            currentColumn = printElems(nonTerminals, columnLength, step, currentColumn, actions);
            printElems(terminals, columnLength, step, currentColumn, actions);
            System.out.println();
            System.out.println(separator);
        }
    }

    public static void printProductions(Grammar grammar) throws Exception {
        System.out.println("Productions:");
        for (int step = 0; step < grammar.getProductionSet().size(); step++) {
            Production production = grammar.getProductionById(step);
            System.out.println(format("%d) %s", step, production.toString()));
        }
        System.out.println();
    }

    private static int printElems(Set<String> nonTerminals, List<Integer> columnLength, int step, int currentColumn, List<Map<String, String>> actions) {
        for (String nonT: nonTerminals) {
            String toPrint = actions.get(step).get(nonT);
            if(toPrint == null) {
                toPrint = "";
            }
            System.out.print(" ");
            int dif = columnLength.get(currentColumn) - toPrint.length();
            for (int i = 0; i < dif / 2; i++) {
                System.out.print(" ");
            }
            System.out.print(toPrint);
            for (int i = 0; i < (dif + 1) / 2; i++) {
                System.out.print(" ");
            }
            System.out.print(" |");
            currentColumn++;
        }
        return currentColumn;
    }

    private static void computeColumnLengths(Set<String> nonTerminals, Set<String> terminals, List<Integer> columnLength, Grammar grammar) {
        columnLength.add(String.valueOf(grammar.getStatesSet().size()).length() + 1);
        for (String nonT : nonTerminals) {
            columnLength.add(max(nonT.length(), "reduce ".length() + columnLength.get(0) - 1));
        }
        for (String t : terminals) {
            columnLength.add(max(t.length(), "reduce ".length() + columnLength.get(0) - 1));
        }
    }

    private static void printTableHeader(Set<String> nonTerminals, Set<String> terminals, List<Integer> columnLength, String separator) {
        System.out.println(separator);
        int currentColumn = 0;
        System.out.print("|");
        for (int i = 0; i < columnLength.get(currentColumn) + 2; i++) {
            System.out.print(" ");
        }
        currentColumn++;
        System.out.print("|");
        currentColumn = printHeader(nonTerminals, columnLength, currentColumn);
        printHeader(terminals, columnLength, currentColumn);
        System.out.println();
        System.out.println(separator);
    }

    private static int printHeader(Set<String> nonTerminals, List<Integer> columnLength, int currentColumn) {
        for (String nonT : nonTerminals) {
            System.out.print(" ");
            int dif = columnLength.get(currentColumn) - nonT.length();
            for (int i = 0; i < dif / 2; i++) {
                System.out.print(" ");
            }
            System.out.print(nonT);
            for (int i = 0; i < (dif + 1) / 2; i++) {
                System.out.print(" ");
            }
            System.out.print(" |");
            currentColumn++;
        }
        return currentColumn;
    }

    private static String getLineSeparator(List<Integer> columnLength) {
        String separator = "+";
        for (Integer len : columnLength) {
            for (int i = 0; i < len + 2; i++) {
                separator += "-";
            }
            separator += "+";
        }
        return separator;
    }

    public static void printFirstAndFollow(Map<String, Set<String>> firstSet, Map<String, Set<String>> followSet) {
        System.out.println("First and Follow for the non-terminals:");
        int columnLen = max("FIRST".length(), "FOLLOW".length());
        for (String nonT : firstSet.keySet()) {
            columnLen = max(columnLen, max(nonT.length(), max(String.valueOf(firstSet.get(nonT)).length(), String.valueOf(followSet.get(nonT)).length())));
        }
        String lineSeparator = getLineSeparator(Arrays.asList(columnLen, columnLen, columnLen));
        System.out.println(lineSeparator);
        printLine(Arrays.asList("", "FIRST", "FOLLOW"), columnLen);
        System.out.println(lineSeparator);
        for (String nonT : firstSet.keySet()) {
            printLine(Arrays.asList(nonT, String.valueOf(firstSet.get(nonT)), String.valueOf(followSet.get(nonT))), columnLen);
            System.out.println(lineSeparator);
        }
        System.out.println();
    }

    private static void printLine(List<String> headers, int columnLen) {
        for (String header : headers) {
            System.out.print("| ");
            int dif = columnLen - header.length();
            for (int i = 0; i < dif / 2; i++) {
                System.out.print(" ");
            }
            System.out.print(header);
            for (int i = 0; i < (dif + 1) / 2; i++) {
                System.out.print(" ");
            }
            System.out.print(" ");
        }
        System.out.println("|");
    }

    public static void printStates(Set<State> statesSet) {
        System.out.println("Canonical Collection:");
        for (State state : statesSet) {
            int columnLen = max(format("State #%d", state.getStateId()).length(), "Transition".length());
            for (Production production : state.getProdDerives()) {
                columnLen = max(columnLen, production.toString().length());
            }
            String lineSeparator = getLineSeparator(Arrays.asList(columnLen, columnLen));
            System.out.println(lineSeparator);
            System.out.print(format("| State #%d", state.getStateId()));
            for (int i = 0; i < columnLen + 1 - format("State #%d", state.getStateId()).length(); i++) {
                System.out.print(" ");
            }
            System.out.print("| Transition");
            for (int i = 0; i < columnLen + 1 - "Transition".length(); i++) {
                System.out.print(" ");
            }
            System.out.println("|");
            System.out.println(lineSeparator);
            for (Production production: state.getProdDerives()) {
                System.out.print("| ");
                System.out.print(production);
                for (int i = 0; i < columnLen - production.toString().length(); i++) {
                    System.out.print(" ");
                }
                System.out.print(" |");
                if (production.canDerivate()) {
                    String toPrint = format(" %s -> %d", production.getCurrentElem(), state.getTransitions().get(production.getCurrentElem()));
                    System.out.print(toPrint);
                    for (int i = 0; i < columnLen + 1 - toPrint.length(); i++) {
                        System.out.print(" ");
                    }
                    System.out.println(" |");
                } else {
                    String toPrint = format(" Prod #%d", production.getProdId());
                    System.out.print(toPrint);
                    for (int i = 0; i < columnLen + 1 - toPrint.length(); i++) {
                        System.out.print(" ");
                    }
                    System.out.println(" |");
                }
            }
            System.out.println(lineSeparator);
            System.out.println();
        }
    }
}
