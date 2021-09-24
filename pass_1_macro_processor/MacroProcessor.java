import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.*;

public class MacroProcessor {
  private String removeLeadingComma(String str) {
    if (str.charAt(str.length() - 1) == ',') {
      return str.substring(0, str.length() - 1);
    }
    return str;
  }

  public static void main(String[] args) throws Exception {

    File file = new File("marco.asm");
    FileWriter mnt = new FileWriter("mnt.txt");
    FileWriter mdt = new FileWriter("mdt.txt");
    FileWriter ala = new FileWriter("ala.txt");

    MacroProcessor macroProcessor = new MacroProcessor();
    BufferedReader bufferedReader = new BufferedReader(new FileReader(file));

    String line = "";
    String completeFile = "";

    while ((line = bufferedReader.readLine()) != null) {
      completeFile += line + "\n";
    }
    System.out.println(completeFile);
    bufferedReader.close();
    StringTokenizer stringTokenizer = new StringTokenizer(completeFile);
    while (stringTokenizer.hasMoreTokens()) {
      String token = macroProcessor.removeLeadingComma(stringTokenizer.nextToken());
      if (token.toUpperCase() == "MACRO") {

        while (token.toUpperCase() != "MEND") {}
      }
    }
  }
}
