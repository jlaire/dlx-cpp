#include "SudokuSolver.hpp"
#include "SudokuType.hpp"

#include <iostream>
#include <string>

int main() {
  std::string input;
  unsigned digit_count = 0;
  SudokuType type;
  SudokuSolver solver(type);
  for (char c; std::cin.get(c);) {
    input.push_back(c);
    if (type.is_cell(c)) {
      ++digit_count;
      if (digit_count == type.size()) {
	std::cout << solver.solve(std::move(input));
	digit_count = 0;
	input.clear();
      }
    }
  }

  std::cout << input;
}
