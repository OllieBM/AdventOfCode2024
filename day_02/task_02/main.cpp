#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
struct Game {
  int id;
  int red;
  int green;
  int blue;
};

int GetMaximumValueFromInput(const std::string &text, std::string match) {

  std::vector<std::string> counts{};
  auto re = std::regex(match);
  std::copy(std::sregex_token_iterator(text.begin(), text.end(), re, 1),
            std::sregex_token_iterator(), std::back_inserter(counts));

  auto it = std::max_element(counts.begin(), counts.end(), [](auto i, auto j) {
    return std::atoi(i.c_str()) < std::atoi(j.c_str());
  });
  if (it == counts.end()) {
    std::cout << " could not find maximum value " << std::endl;
    exit(-1);
  }
  return std::atoi(it->c_str());
}

int main(void) {

  auto ifs = std::ifstream("input.txt");
  if (!ifs.is_open()) {
    std::cout << "Error opening file.\n";
  }

  unsigned long long game_counter = int(0);
  for (std::string line; std::getline(ifs, line, '\n');) {

    // std::regex idMatch("Game ([0-9]+):");
    // std::smatch ids;
    // std::regex_search(line, ids, idMatch); // check bool return for validity

    auto red = GetMaximumValueFromInput(line, std::string{"([0-9]+) red"});
    auto green = GetMaximumValueFromInput(line, std::string{"([0-9]+) green"});
    auto blue = GetMaximumValueFromInput(line, std::string{"([0-9]+) blue"});
    std::cout << "[" << line << "] :\t";
    std::cout << "reds " << red << "\t";
    std::cout << "green " << green << "\t";
    std::cout << "blue " << blue << "\n";

    unsigned long long power = red * green * blue;
    game_counter += power;
  }
  std::cout << "sum of all possible game powers is  " << game_counter << "\n";
  return game_counter;
}
