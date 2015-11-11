#include "Sudoku.hpp"

#include <ctype.h>
#include <iostream>
#include <string>

int main() {
  std::string input;
  unsigned digit_count = 0;
  Sudoku sudoku;
  for (char c; std::cin.get(c);) {
    input.push_back(c);
    if (sudoku.is_cell(c)) {
      ++digit_count;
      if (digit_count == sudoku.size()) {
	std::cout << sudoku.solve(std::move(input));
	digit_count = 0;
	input.clear();
      }
    }
  }

  std::cout << input;
}
