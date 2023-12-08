#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <regex>
#include <string>
#include <unordered_set>

using namespace std;

map<size_t, size_t> Solution(string filename) {
  auto ifs = ifstream(filename);
  if (!ifs.is_open()) {
    cout << "Error opening file.\n";
    exit(-1);
  }

  // game id to winning numbers
  map<size_t, size_t> games_to_wins{};

  for (string line; getline(ifs, line, '\n');) {

    unordered_set<string> winning{};
    vector<string> our_winning_numbers{};
    auto start = find(line.begin(), line.end(), ':');
    auto sep = find(start, line.end(), '|');

    auto id = size_t{};
    auto idRe = regex("^Card\\s+([0-9]+):");
    for_each(sregex_token_iterator(line.begin(), sep, idRe, 1),
             sregex_token_iterator(), [&id](auto i) {
               id = static_cast<size_t>(atoi(i.str().c_str()));
             });

    auto re = regex("([0-9]+)");
    for_each(sregex_token_iterator(start, sep, re, 1), sregex_token_iterator(),
             [&winning](auto i) { winning.insert(i.str()); });
    for_each(sregex_token_iterator(sep, line.end(), re, 1),
             sregex_token_iterator(), [&winning, &our_winning_numbers](auto i) {
               if (winning.find(i.str()) != winning.end()) {
                 our_winning_numbers.push_back(i.str());
               }
             });

    games_to_wins.insert({id, our_winning_numbers.size()});
  }
  games_to_wins.erase(0);
  return games_to_wins;
}

int main(void) {

  //   map<string, vector<int>> game_to_wins{};
  queue<size_t> pending{};
  //   pending.push(1UL);
  vector<size_t> visited{};

  auto game_to_wins = Solution("input.txt");
  for (size_t i = 1; i <= game_to_wins.size(); i++) {
    pending.push(i);
  }
  while (!pending.empty()) {
    auto i = pending.front();
    pending.pop();
    visited.push_back(i);

    auto it = game_to_wins.find(i);
    if (it != game_to_wins.end()) {
      auto wins = it->second;
      // std::cout << "card [" << it->first << "]"
      //           << " won " << wins << " times\n";
      for (auto j = 1; j < wins + 1; ++j) {
        pending.push(j + i);
      }
    }
    // if (visited.size() > 50) {
    //   break;
    // }
  }
  std::cout << "number of wins: " << visited.size() << std::endl;
}