package util;

import entity.Production;
import org.apache.commons.io.FileUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import static java.lang.String.format;

public class UtilsReader {
    private static Production buildStartPoint(String origin) throws Exception {
        return new Production(format("%1$s' %1$s", origin), 0, -1);
    }

    public static Set<Production> readProductionsFromFile(String filePath) throws Exception {
        Set<Production> productions = new HashSet<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line = br.readLine();
            if (line != null) {
                Production firstProd = new Production(line, 1, -1);
                Production startPoint = buildStartPoint(firstProd.getOrigin());
                productions.add(startPoint);
                productions.add(firstProd);
                int curId = 2;
                while ((line = br.readLine()) != null) {
                    productions.add(new Production(line, curId, -1));
                    curId++;
                }
            } else {
                throw new Exception("Unable to find any productions in file.");
            }
        }
        return productions;
    }

    public static String readValidSequence(String filePath) throws IOException {
        File file = new File(filePath);
        String fileAsString = FileUtils.readFileToString(file);
        return fileAsString.replaceAll("\\r"," ").replaceAll("\\n"," ").replaceAll("\\t", " ").replaceAll("[ ]+", " ").trim();
    }

}
