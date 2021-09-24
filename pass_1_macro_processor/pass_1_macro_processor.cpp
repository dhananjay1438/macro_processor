#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <vector>

/*
 * Right now indexing is 0th based, change it to 1st based if required
 *
 * Created until now
 * 1) Macro name table (macro name, index) {
 *  class {
 *  	name;
 *  	index;
 *  }
 * }
 * 2) Macro definition table (complete lines from macro definition to MEND)  {
 * 		std::vector<std::string>
 * }
 *
 * 3) Argument array list (macro name, argument list) {
 * 		vector<std::pair<std::string> std::vector<string>>>
 * }
 */

std::string remove_trailing_whitespaces(std::string string) {
  size_t trailing_whitespaces = 0;
  if (string.size() > 0) {
    for (auto &ch : string) {
      if (std::isspace(ch)) {
        trailing_whitespaces++;
      } else {
        break;
      }
    }
  }
  return string.substr(trailing_whitespaces, string.size());
}
// Macro name table
class MNT {
public:
  std::string name;
  int index;
  int get_index();
  std::string get_name();
  MNT(std::string, int);
};

MNT::MNT(std::string name, int index) {
  this->index = index;
  if (name.size() > 8) {
    this->name = name.substr(0, 7);
  } else {
    this->name = name;
  }
}
int MNT::get_index() { return index; }
std::string MNT::get_name() { return name; }

int main(void) {
  std::fstream file;
  std::string string;
  // Macro definition table
  std::vector<std::string> mdt;
  // Macro name table
  std::vector<MNT> mnt;
  // Argument array list
  std::vector<std::pair<std::string, std::vector<std::string>>>
      argument_array_list;

  std::smatch matches;
  std::regex regex("(&)[^ ]*"); // Regex to find word starting with & (i.e
                                // parameters for macro)

  bool macro_found = false;

  file.open("marco.asm", std::ios::in);

  if (!file) {
    std::cout << "File can't be opened\n";
    return EXIT_FAILURE;
  }

  /*
   * Checking if string(line) starts with MACRO, if it starts with macro adding
   * lines to mdt until MEND is found
   * Creating a macro definition table
   * Code is written assuming that MACRO keyword and macro name are written on
   * the same line, and MEND is written on a seperate line
   */
  while (!file.eof()) {
    std::getline(file, string);

    string = remove_trailing_whitespaces(string);
    if (string.find("MACRO") != std::string::npos) {
      macro_found = true;
    }
    if (macro_found == true) {
      if (string.compare("MEND") != 0) {
        if (!string.empty()) {
          mdt.push_back(string);
        }
      } else {
        mdt.push_back(string);
        macro_found = false;
      }
    }
  }
  // Creating a Macro name table
  /*
   * TODO: Find a better way to parse lines instead of using regex, regex is way
   * too slow (takes around 2.7 seconds to run and generates 20k+ plus lines of
   * assembly)
   */
  for (size_t i = 0; i < mdt.size(); i++) {
    if (mdt[i].rfind("MACRO", 0) == 0) {
      // Starting for 6th index as name of macro name will start from 6th
      // index(because MACRO consists of 5 letters and 1 letter is "space", so
      // total 6 letters i.e for 0 to 5th index. Therefore, 6th index) Assuming
      // keyword macro and marco name are on the same line
      size_t pos = mdt[i].find(" ", 6);
      std::string macro_name = mdt[i].substr(6, pos - 6);
      mnt.push_back(MNT(macro_name, static_cast<int>(i)));

      // Creating argument array list
      std::vector<std::string> argument_list;
      // Not using mdt[i] as regex changes the string contents, instead creating
      // a dummy variable
      std::string mdt_i = mdt[i];
      while (std::regex_search(mdt_i, matches, regex)) {
        std::string arg = matches[0];
        if (arg.at(arg.size() - 1) == ',') {
          argument_list.push_back(arg.substr(0, arg.size() - 1));
        } else {
          argument_list.push_back(arg);
        }
        mdt_i = matches.suffix().str();
      }
      argument_array_list.push_back(std::make_pair(macro_name, argument_list));
    }
  }

  // Displaying the data

  std::cout << std::left << std::setfill('*') << std::setw(50)
            << "Macro Defintion Table\n";
  std::cout << "\n";
  for (auto &x : mdt) {
    std::cout << x << "\n";
  }
  std::cout << "\n\n"
            << std::setfill('*') << std::setw(50) << "Macro Name Table\n";
  std::cout << "\n";
  for (auto &x : mnt) {
    std::cout << x.get_index() << " " << x.get_name() << "\n";
  }
  std::cout << "\n\n"
            << std::setfill('*') << std::setw(50) << "Argument array list\n";
  std::cout << "\n";
  for (auto x : argument_array_list) {
    std::cout << x.first << ": ";
    for (auto i : x.second) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
