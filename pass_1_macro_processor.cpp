#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
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
 * 2) Macro definition table (complete lines from macro definition to mend)  {
 * 		std::vector<std::string>
 * }
 *
 * 3) Argument array list (macro name, argument list) {
 * 		std::string to hold macro name
 * 		and std::vector<string> to hold parameters
 * 		vector<std::pair<std::string> std::vector<string>>>
 * }
 */

const std::string WHITESPACE = " \t";
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

std::string create_mnt(std::vector<std::string> &mdt, size_t i,
                       std::vector<MNT> &mnt, bool on_next_line) {
  std::string macro_name;
  if (on_next_line) {
    size_t pos = mdt[i].find(" ");
    macro_name = mdt[i].substr(0, pos);
    mnt.push_back(MNT(macro_name, static_cast<int>(i)));
  } else {
    size_t pos = mdt[i].find(" ", 6);
    macro_name = mdt[i].substr(6, pos - 6);
    mnt.push_back(MNT(macro_name, static_cast<int>(i)));
  }
  return macro_name;
}

std::string remove_trailing_whitespaces(std::string string) {
  if (string.length() > 0) {
    size_t last = string.find_last_not_of(WHITESPACE);
    string.erase(last + 1);
    return string;
  }
  return string;
}

std::string remove_leading_whitespaces(std::string string) {
  if (string.length() > 0) {
    size_t first = string.find_first_not_of(WHITESPACE);
    return string.substr(first);
  }
  return string;
}

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

  bool macro_found = false;

  file.open("macro.asm", std::ios::in);

  if (!file) {
    std::cout << "File can't be opened\n";
    return EXIT_FAILURE;
  }

  /*
   * Checking if string(line) starts with macro, if it starts with macro adding
   * lines to mdt until mend is found
   * Creating a macro definition table
   * Code is written assuming that macro keyword and macro name are written on
   * the same line, and mend is written on a seperate line
   */
  while (!file.eof()) {
    std::getline(file, string);

    string = remove_leading_whitespaces(string);
    string = remove_trailing_whitespaces(string);

    std::string lower_cased_string = string;
    std::transform(lower_cased_string.begin(), lower_cased_string.end(),
                   lower_cased_string.begin(), ::tolower);
    if (lower_cased_string.find("macro") != std::string::npos) {
      macro_found = true;
    }
    if (macro_found == true) {
      if (lower_cased_string.compare("mend") != 0) {
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
  for (size_t i = 0; i < mdt.size(); i++) {
    if (mdt[i].rfind("macro", 0) == 0) {
      std::string macro_name;
      if (mdt[i].compare("macro") == 0) {
        // If whole line only contains "MACRO" in it
        macro_name = create_mnt(mdt, i + 1, mnt, true);
        i++;
      } else {
        // If line contains macro plus macro name and arguments
        macro_name = create_mnt(mdt, i, mnt, false);
      }

      // Creating argument array list (it is newly created for each macro)
      std::vector<std::string> argument_list;
      // Not using mdt[i] as regex changes the string contents, instead creating
      // a dummy variable
      std::string mdt_i = mdt[i];
      size_t first_argument = mdt_i.find_first_of('&');
      mdt_i.erase(0, first_argument);

      std::stringstream sstream(mdt_i);
      std::string argument;
      while (std::getline(sstream, argument, ',')) {
        argument_list.push_back(argument);
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
