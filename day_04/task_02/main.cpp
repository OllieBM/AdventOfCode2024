#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <regex>
#include <string>
#include <unordered_set>

using namespace std;

unordered_map<string, vector<string>> Solution(string filename) {
  auto ifs = ifstream(filename);
  if (!ifs.is_open()) {
    cout << "Error opening file.\n";
    exit(-1);
  }

  // game id to winning numbers
  unordered_map<string, vector<string>> games_to_wins{};

  for (string line; getline(ifs, line, '\n');) {

    unordered_set<string> winning{};
    vector<string> our_winning_numbers{};
    auto start = find(line.begin(), line.end(), ':');
    auto sep = find(start, line.end(), '|');

    string id = "";
    auto idRe = regex("^Card ([0-9]+):");
    for_each(sregex_token_iterator(line.begin(), sep, idRe, 1),
             sregex_token_iterator(), [&id](auto i) { id = i.str(); });

    auto re = regex("([0-9]+)");
    for_each(sregex_token_iterator(start, sep, re, 1), sregex_token_iterator(),
             [&winning](auto i) { winning.insert(i.str()); });
    for_each(sregex_token_iterator(sep, line.end(), re, 1),
             sregex_token_iterator(), [&winning, &our_winning_numbers](auto i) {
               if (winning.find(i.str()) != winning.end()) {
                 our_winning_numbers.push_back(i.str());
               }
             });

    games_to_wins.insert({id, our_winning_numbers});
  }
  return games_to_wins;
}

int main(void) {

  //   unordered_map<string, vector<int>> game_to_wins{};
  queue<string> pending{};
  vector<string> visited{};
  auto game_to_wins = Solution("test_input.txt");
  for (auto it : game_to_wins) {
    // cout << "Card " << it.first << ": ";
    // for (auto v : it.second) {
    //   cout << v << " ";
    // }
    // cout << "\n";
    visited.push_back(it.first);
    for_each(it.second.begin(), it.second.end(),
             [&pending](auto i) { pending.push(i); });
  }
  while (!pending.empty()) {
    auto curr = pending.front();

    // keep track of the number of wins
    visited.push_back(curr);

    auto it = game_to_wins.find(curr);
    // it should never be == game_to_wins.end()
    assert(it != game_to_wins.end());
    // add the wins to the pending
    for_each(it->second.begin(), it->second.end(),
             [&pending](auto i) { pending.push(i); });
    pending.pop();
  }
  std::cout << "number of wins: " << visited.size() << std::endl;
}