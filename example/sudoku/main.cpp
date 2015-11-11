#include "Sudoku.hpp"

#include <ctype.h>
#include <iostream>
#include <string>

int main() {
  std::string input;
  unsigned digit_count = 0;
  for (char c; std::cin.get(c);) {
    input.push_back(c);
    if (::isdigit(c) || c == '.') {
      ++digit_count;
      if (digit_count == 9 * 9) {
	std::cout << Sudoku::solve(std::move(input));
	digit_count = 0;
	input.clear();
      }
    }
  }

  std::cout << input;
}
