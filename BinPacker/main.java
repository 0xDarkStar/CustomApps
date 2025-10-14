import java.util.HashMap;
import java.util.List;
import java.util.Map;

import calculator.Run;
import calculator.RunCalculator;
import formatter.Formatter;
import parser.FileParser;

public class main {
    public static void main(String[] args) {
        String fileName = args[0];
        
        FileParser organizer = new FileParser();
        // Get the list from the file
        HashMap<String, Integer> list = organizer.getListFromFile(fileName);
        List<Map.Entry<String,Integer>> orderedList = organizer.sortList(list);
        // Figure out what are the optimized runs.
        RunCalculator runCalc = new RunCalculator();
        List<Run> allRuns = runCalc.calculateRuns(orderedList, organizer.storageSpace);
        // Print it out for the user
        Formatter textFormat = new Formatter();
        textFormat.writeRunsToFile("block", allRuns);;
    }
}
