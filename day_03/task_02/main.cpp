#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <vector>

std::vector<std::vector<char>> get_all_numbers(
    std::string filename,
    std::map<std::pair<int /*y*/, int /*x*/>, size_t /*vector position*/>
        &mapping,
    std::vector<std::pair<int, int>> &gears, std::vector<int> &numbers) {

  numbers.clear();
  gears.clear();
  mapping.clear();

  auto ifs = std::ifstream(filename);
  if (!ifs.is_open()) {
    std::cout << "Error opening file.\n";
    exit(-1);
  }

  std::vector<std::vector<char>> grid;

  auto re = std::regex("([0-9]+)");
  int y = 0;
  for (std::string line; std::getline(ifs, line, '\n'); y++) {

    grid.push_back(std::vector<char>(line.begin(), line.end()));
    auto it = line.begin();
    auto end = line.end();

    while (it != end) {
      it = std::find_if(it, end, static_cast<int (*)(int)>(std::isdigit));
      auto n =
          std::find_if_not(it, end, static_cast<int (*)(int)>(std::isdigit));

      if (n != end) {
        // auto x0 = static_cast<int>(std::distance(line.begin(), it));
        // auto x1 = static_cast<int>(std::distance(line.begin(), n) - 1);
        // std::cout << "[" << x0 << "," << x1 << "] " << std::string{it, n}
        //           << std::endl;

        auto found = std::string{it, n};
        // std::cout << "found " << found << std::endl;
        numbers.push_back(std::atoi(found.c_str()));
        for (auto x = static_cast<int>(std::distance(line.begin(), it));
             x < static_cast<int>(std::distance(line.begin(), n)); x++)
          mapping.insert({{y, x}, numbers.size() - 1});
      }

      it = n;
    }

    for (int x = 0; x < line.size(); x++) {
      if (line[x] == '*') {
        gears.push_back({y, x});
      }
    }
  }
  return grid;
}

int main(void) {

  auto mapping = std::map<std::pair<int, int>, size_t>{};
  auto gears = std::vector<std::pair<int, int>>{};
  auto numbers = std::vector<int>{};
  auto grid = get_all_numbers("input.txt", std::ref(mapping), std::ref(gears),
                              std::ref(numbers));

  std::cout << numbers.size() << " total numbers." << std::endl;
  std::cout << mapping.size() << " total number characters " << std::endl;
  std::cout << gears.size() << " total number of gears " << std::endl;

  unsigned long long total = 0;
  for (auto gear : gears) {
    int x = 0;
    int y = 0;
    std::tie(y, x) = gear;
    int ax = std::max(x - 1, 0);
    int ay = std::max(y - 1, 0);
    int bx = std::min(x + 2, static_cast<int>(grid[y].size()));
    int by = std::min(y + 2, static_cast<int>(grid.size()));

    std::set<int /*index*/> matches{};
    for (auto iy = ay; iy < by; iy++) {
      for (auto ix = ax; ix < bx; ix++) {
        // std::cout << "[" << grid[iy][ix] << "]";
        auto it = mapping.find({iy, ix});
        if (it != mapping.end()) {
          // std::cout << numbers[it->second] << std::endl;
          matches.insert(it->second);
        }
      }
      // std::cout << std::endl;
    }

    if (matches.size() == 2) {
      int delta = 1;
      for (auto i : matches) {
        delta *= numbers[i];
      }
      total += delta;
    }
  }
  std::cout << " should not be : " << 79570642 << std::endl;
  std::cout << "total " << total << std::endl;
  return 0;
}
