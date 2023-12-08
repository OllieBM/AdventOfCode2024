#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <unordered_set>

using namespace std;

unsigned long long Solution(string filename) {
  auto ifs = ifstream(filename);
  if (!ifs.is_open()) {
    cout << "Error opening file.\n";
    exit(-1);
  }

  unsigned long long total = 0;

  for (string line; getline(ifs, line, '\n');) {

    std::unordered_set<std::string> winning{};
    std::vector<std::string> our_winning_numbers{};
    auto start = find(line.begin(), line.end(), ':');
    auto sep = find(start, line.end(), '|');

    auto re = regex("([0-9]+)");
    for_each(std::sregex_token_iterator(start, sep, re, 1),
             std::sregex_token_iterator(),
             [&winning](auto i) { winning.insert(i.str()); });
    for_each(sregex_token_iterator(sep, line.end(), re, 1),
             sregex_token_iterator(), [&winning, &our_winning_numbers](auto i) {
               if (winning.find(i.str()) != winning.end()) {
                 our_winning_numbers.push_back(i.str());
               }
             });

    // for (auto i : winning) {
    //   cout << i << " ";
    // }
    // cout << " | ";
    // for (auto i : our_winning_numbers) {
    //   cout << i << " ";
    // }
    if (!our_winning_numbers.empty()) {
      const auto p = (our_winning_numbers.size() - 1);
      total += pow(2, p);
    }
  }
  return total;
}

int main(void) { std::cout << Solution("input.txt") << std::endl; }