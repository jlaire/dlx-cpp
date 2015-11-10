#include "Sudoku.hpp"

#include <ctype.h>
#include <iostream>
#include <string>

int main() {
  std::string grid;
  for (char c; std::cin >> c;) {
    if (::isdigit(c)) {
      grid.push_back(c);
    }
    else if (c == '.') {
      grid.push_back('0');
    }
    if (grid.size() == 81) {
      std::cout << Sudoku::solve(std::move(grid)) << '\n';
      grid.clear();
    }
  }

  if (!grid.empty()) {
    std::cerr << "Trailing characters in input: " << grid << '\n';
  }
}
