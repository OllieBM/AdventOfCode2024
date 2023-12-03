
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(void) {

  std::ifstream ifs("input.txt");
  auto lines = std::vector<std::string>{};
  for (std::string line; std::getline(ifs, line, '\n');) {
    lines.push_back(line);
  }

  // list of numbers to search
  const std::vector<std::string> numbers{
      "1",   "2",   "3",     "4",    "5",    "6",   "7",     "8",     "9",
      "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
  };
  // the numerical value of those numbers
  const std::vector<int> translation{1, 2, 3, 4, 5, 6, 7, 8, 9,
                                     1, 2, 3, 4, 5, 6, 7, 8, 9};

  long long total = 0;
  for (auto &line : lines) {

    int bestFirstPos = line.size();
    int bestLastPos = line.size();
    int first = 0;
    int last = 0;
    for (size_t n = 0; n < numbers.size(); ++n) {
      auto pos = line.find(numbers[n]);
      if (pos != std::string::npos && pos <= bestFirstPos) {
        bestFirstPos = pos;
        first = translation[n];
      }

      // reverse check
      auto rline = std::string(line.rbegin(), line.rend());
      auto rnumber = std::string{numbers[n].rbegin(), numbers[n].rend()};
      pos = rline.find(rnumber);
      if (pos != std::string::npos && pos <= bestLastPos) {
        bestLastPos = pos;
        last = translation[n];
      }
    }

    /* Debug
    std::cout << line << " ->  " << first << " \n"
              << std::string(line.rbegin(), line.rend()) << " -> " << last
              << std::endl;
    */

    // TODO:
    // check if first&last and best positions are valid and not default
    total += ((first * 10) + last);
  }

  std::cout << total << std::endl;
  return 0;
}
