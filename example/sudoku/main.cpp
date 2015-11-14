#include "SudokuSolver.hpp"
#include "SudokuType.hpp"

#include <unistd.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  // TODO: Multiple format options:
  //        - same as in input,
  //        - pretty print (as now),
  //        - compact (only digits, one row per line),
  //        - super compact (like sudoku17).

  bool opt_pretty_print = false;
  bool opt_print_initial = false;
  unsigned opt_size = 9;

  for (int opt; (opt = ::getopt(argc, argv, "ips:")) != -1;) {
    switch (opt) {
      case 'i':
	opt_print_initial = true;
	break;
      case 'p':
	opt_pretty_print = true;
	break;
      case 's':
	opt_size = std::stoi(::optarg);
	break;
      default:
	throw std::logic_error("unhandled case in getopt loop");
    }
  }

  std::string input;
  unsigned digit_count = 0;
  auto type = std::make_shared<SudokuType>(opt_size);
  for (char c; std::cin.get(c);) {
    input.push_back(c);
    if (!type->is_cell(c)) {
      continue;
    }

    if (++digit_count < type->size()) {
      continue;
    }

    Sudoku sudoku(type, input);
    if (opt_pretty_print) {
      sudoku.set_template(sudoku.default_template());
    }
    if (opt_print_initial) {
      std::cout << sudoku.to_string() << '\n';
    }

    auto solved = SudokuSolver().solve(sudoku);
    if (opt_pretty_print) {
      solved.set_template(sudoku.default_template());
    }
    std::cout << solved.to_string() << '\n';

    digit_count = 0;
    input.clear();
  }
}
