import analyzer.SlrAnalyzer;

import static util.UtilsReader.readValidSequence;

public class Main {

    public static void main(String[] args) {
        try {
            SlrAnalyzer analyzer = new SlrAnalyzer("data/grammar.txt");
            String seqToAnalyze = readValidSequence("data/sequence.txt");
            System.out.println(analyzer.checkSequence(seqToAnalyze));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
