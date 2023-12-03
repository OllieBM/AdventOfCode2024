
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// std::stringstream read_file(std::string filename) {
//   std::ifstream ifs(filename);
//   std::stringstream buffer;
//   buffer << ifs.rdbuf();
//   return buffer;
// }

int main(void) {

  // const char *input = "1abc2\n"
  //                     "pqr3stu8vwx\n"
  //                     "a1b2c3d4e5f\n"
  //                     "treb7uchet";
  // auto input = read_file("input.txt");
  std::ifstream ifs("input.txt");

  // 1. split string into lines
  // 2. scan each line for the first & last digits
  // 3. convert those digits into a single number;
  // 4. sum numbers from all lines;

  auto lines = std::vector<std::string>{};
  for (std::string line; std::getline(ifs, line, '\n');) {
    // std::cout << "adding line: " << line << std::endl;
    lines.push_back(line);
  }

  long long total = 0;
  auto isDigit = [](int c) { return std::isdigit(static_cast<char>(c)); };
  for (auto &line : lines) {

    auto first = std::find_if(line.begin(), line.end(), isDigit);
    auto last = std::find_if(line.rbegin(), line.rend(), isDigit);
    // std::cout << line << " -> " << *first << *last << std::endl;
    auto v = std::atoi(std::string{*first, *last}.c_str());
    total += v;
    std::cout << line << " -> " << v << std::endl;
  }
  std::cout << total << std::endl;
  return 0;
}

// problems:
// possiblity of trying to create too large a number