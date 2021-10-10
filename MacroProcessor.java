import java.io.*;
import java.util.*;
// TODO: Using space between comma causes program to be whitespace(indentention) sensitive, fix it

// Need to create temp file properly right not it holds "macro_name label, arguments" but it should
// have "label macro_name, arguments"
class MacroNameTable {
  public String name;
  public int index;

  public MacroNameTable(int index, String name) {
    this.index = index;
    this.name = name;
  }

  public int getIndex() {
    return index;
  }

  public String getName() {
    return name;
  }
}

public class MacroProcessor {

  private void createOutputFile(List<String> mdt) throws Exception {

    PrintWriter printWriter = new PrintWriter("output.asm");
    printWriter.close();
    BufferedReader reader = new BufferedReader(new FileReader("macro.asm"));
    BufferedWriter writer = new BufferedWriter(new FileWriter("temp.asm"));

    boolean isToBeDeleted = false;
    String currentLine = "";

    while ((currentLine = reader.readLine()) != null) {
      for (String toDelete : mdt) {
        if (currentLine.trim().equals(toDelete)) {
          isToBeDeleted = true;
        }
      }
      if (isToBeDeleted) {
        isToBeDeleted = false;
        continue;
      } else {
        writer.write(currentLine + "\n");
      }
    }
    reader.close();
    writer.close();
  }

  private void pass2(List<String> mdt, List<MacroNameTable> mnt, HashMap<String, List<String>> ala)
      throws Exception {

    // TODO: Find a way where I won't require temporary file (temp.asm)
    File file = new File("temp.asm");
    BufferedReader bufferedReader = new BufferedReader(new FileReader(file));
    BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter("output.asm", true));
    ;
    List<String> completeFile = new ArrayList<String>();
    String line = "";
    while ((line = bufferedReader.readLine()) != null) {
      if (line.length() > 0) {
        completeFile.add(line + "\n");
      }
    }

    boolean macroFound = false;
    for (String currentLine : completeFile) {
      // Running loop through mnt which contains definition and body of macros
      for (int i = 0; i < mnt.size(); i++) {
        // checks if macro name is present in a line
        if (currentLine.contains(mnt.get(i).getName())) {

          // If we found macro name in current line we set macroFound to true so in else block we
          // canknow if macro is found or not, if it is we do not print extra line else we print it
          macroFound = true;

          // We have add 1 here as we don't want to print macro definition, we only want to print
          // statements inside it
          int index = mnt.get(i).getIndex() + 1;
          String macro_name = mnt.get(i).getName();

          List<String> oldTokens = ala.get(macro_name);
          List<String> newTokens = new ArrayList<String>();
          newTokens.addAll(Arrays.asList(currentLine.split(" |,")));

          // If first char is "&" it means there is argument before macro name so we remove the
          // macro name (i.e 1st index) else we simply remove first token i.e macro name
          if (mdt.get(i).startsWith("&")) {
            newTokens.remove(1);
          } else {
            newTokens.remove(0);
          }
          // index - 1 = defintion of macro. We need to check if there is argument(label) before
          // macro name
          if (mdt.get(index - 1).startsWith("&")) {
            bufferedWriter.write(newTokens.get(0) + " ");
          }

          // Adding lines into output file until we don't find MEND
          while (!mdt.get(index).equals("MEND")) {
            String mdtLine = mdt.get(index);
            for (int tokenIndex = 0; tokenIndex < newTokens.size(); tokenIndex++) {
              mdtLine = mdtLine.replace(oldTokens.get(tokenIndex), newTokens.get(tokenIndex));
            }
            if (mdtLine.endsWith("\n")) {
              bufferedWriter.write(mdtLine);
            } else {
              bufferedWriter.write(mdtLine + "\n");
            }
            index++;
          }
        }
      }
      if (!macroFound) {
        bufferedWriter.write(currentLine);
      }
      macroFound = false;
    }

    bufferedReader.close();
    bufferedWriter.close();
  }

  private List<String> createMDT() throws Exception {
    File file = new File("macro.asm");
    BufferedReader bufferedReader = new BufferedReader(new FileReader(file));

    List<String> mdt = new ArrayList<String>();
    String line = "";
    boolean macroFound = false;
    // Currently only MACRO and MEND is accepted i.e it is case sensitive, to make it case
    // insensitive just add .toUpperCase() in each if condition
    // Created macro definition table
    // Macro deifintion contains "MACRO" as a string as well
    while ((line = bufferedReader.readLine()) != null) {
      if (macroFound == true) {
        mdt.add(line.trim());
        if (line.trim().equals("MEND")) {
          macroFound = false;
        }
      }
      if (line.trim().equals("MACRO")) {
        macroFound = true;
        mdt.add(line.trim());
      }
    }
    bufferedReader.close();
    return mdt;
  }

  public static void main(String[] args) throws Exception {

    MacroProcessor macroProcessor = new MacroProcessor();
    List<String> mdt = macroProcessor.createMDT();
    List<MacroNameTable> mnt = new ArrayList<MacroNameTable>();
    HashMap<String, List<String>> ala = new HashMap<String, List<String>>();
    for (int i = 0; i < mdt.size(); i++) {
      if (mdt.get(i).equals("MACRO")) {
        // incrementing i here so we don't have to increment i when trying to access index of m
        i++;
        String line = mdt.get(i);
        String[] tokens = line.split(" |,");
        List<String> arguments = new ArrayList<String>();
        String macroName = "";
        for (String token : tokens) {
          if (token.startsWith("&")) {
            arguments.add(token);
          } else {
            macroName = token;
          }
        }
        // Assigning line to become linve from next char after whitespace is found (in order to find
        // argumets which are seperated by ',')
        mnt.add(new MacroNameTable(i, macroName));
        ala.put(macroName, arguments);
      }
    }
    System.out.println("**********Macro definition table**********");
    System.out.println(mdt);

    System.out.println("\n**********Macro name table**********");
    for (MacroNameTable temp : mnt) {
      System.out.println(temp.getName() + " " + temp.getIndex());
    }
    System.out.println("\n**********Argument array list**********");
    System.out.println(ala);

    macroProcessor.createOutputFile(mdt);
    macroProcessor.pass2(mdt, mnt, ala);
  } // Main method
}
