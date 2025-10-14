package formatter;
import java.util.List;

import calculator.Run;
import calculator.RunItem;

import java.io.FileWriter;
import java.io.IOException;

public class Formatter {
    private FileWriter writer;
    public void printRuns(List<Run> runs) {
        for (Run run : runs) {
            System.out.println(
                "Run " + run.getRunStartNumber() +
                "-" + run.getRunEndNumber() +
                " (" + run.getTotalUsed() +
                ") (Remainder: " + run.getRunRemainder() + ")"
            );
        }
    }

    public void writeRunsToFile(String outputType, List<Run> runs) {
        switch (outputType) {
            case "flow":
                writeFlowRuns(runs);
                break;
            case "block":
                writeBlockRuns(runs);
            default:
                writeBlockRuns(runs);
                break;
        }
    }

    public void writeFlowRuns(List<Run> runs) {

    }

    public void writeBlockRuns(List<Run> runs) {
        try {
            writer = new FileWriter("BlockRuns.txt");

            for (Run run : runs) {
                String currBlock = "";
                if (run.isFull()) {
                    // A FULL run
                    if (run.getRunRemainder() > 0) {
                        currBlock += String.format("\nRun %d-%d (%d):    (Remainder: %d)\n",
                            run.getRunStartNumber(),
                            run.getRunEndNumber(),
                            run.getTotalUsed(),
                            run.getRunRemainder());
                    } else {
                        currBlock += String.format("\nRun %d-%d (%d):\n",
                            run.getRunStartNumber(),
                            run.getRunEndNumber(),
                            run.getTotalUsed());
                    }
                } else {
                    // A non-FULL run (possibly remainder run?)
                    if (run.getRunRemainder() > 0) {
                        currBlock += String.format("\nRun %d (%d):    (Remainder: %d)\n",
                            run.getRunStartNumber(),
                            run.getTotalUsed(),
                            run.getRunRemainder());
                    } else {
                        currBlock += String.format("\nRun %d (%d):\n",
                            run.getRunStartNumber(),
                            run.getTotalUsed());
                    }
                }
                int maxNameSize = run.getLargestNameSize();
                for (RunItem item : run.getItems()) {
                    currBlock += String.format("  - %-"+maxNameSize+"s %d\n",
                        item.getName(),
                        item.getQuantity());
                }
                writer.write(currBlock);
            }
            writer.close();
        } catch (IOException e) {
            // TODO: handle exception
        }
    }
}
