#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::vector<char>> GetGrid(std::string filename) {

  auto ifs = std::ifstream(filename);
  if (!ifs.is_open()) {
    std::cout << "Error opening file.\n";
    exit(-1);
  }

  std::vector<std::vector<char>> grid;
  for (std::string line; std::getline(ifs, line, '\n');) {
    grid.push_back(std::vector<char>(line.begin(), line.end()));
  }

  return grid;
}
void PrintGrid(const std::vector<std::vector<char>> grid) {
  for (auto row : grid) {
    for (auto c : row) {
      std::cout << '[' << c << ']';
    }
    std::cout << "\n";
  }
}

int main() {
  auto grid = GetGrid("input.txt");

  long long total = 0;
  for (int y = 0; y < grid.size(); ++y) {

    auto begin = grid[y].begin();
    auto it = begin;
    auto end = grid[y].end();

    while (it != end) {
      const auto isDigit = static_cast<int (*)(int)>(std::isdigit);
      it = std::find_if(it, end, isDigit);
      auto n = std::find_if_not(it, end, isDigit);

      int min_x = std::max(0, static_cast<int>(std::distance(begin, it) - 1));
      int max_x = std::min(static_cast<int>(grid[y].size() - 1),
                           static_cast<int>(std::distance(begin, n) + 1));
      int min_y = std::max(0, y - 1);
      int max_y = std::min(static_cast<int>(grid.size() - 1), y + 1);

      std::string found{it, n};
      for (auto j = min_y; j <= max_y; j++) {
        if (std::any_of(grid[j].begin() + min_x, grid[j].begin() + max_x,
                        [](char c) { return !std::isalnum(c) && c != '.'; })) {
          // std::cout << "valid " << found << std::endl;
          total += std::atoi(found.c_str());
        }
      }

      it = n;
    }
  }
  std::cout << total << std::endl;
  return 0;
}