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

  enum Format { DEFAULT, ONELINE, COMPACT, PRESERVE };
  Format opt_format = DEFAULT;
  bool opt_print_initial = false;
  unsigned opt_size = 9;

  for (int opt; (opt = ::getopt(argc, argv, "if:s:")) != -1;) {
    switch (opt) {
      case 'i':
	opt_print_initial = true;
	break;
      case 'f':
	if (::optarg == std::string("default")) {
	  opt_format = DEFAULT;
	}
        else if (::optarg == std::string("oneline")) {
	  opt_format = ONELINE;
	}
	else if (::optarg == std::string("compact")) {
	  opt_format = COMPACT;
	}
	else if (::optarg == std::string("preserve")) {
	  opt_format = PRESERVE;
	}
	else {
	  std::cerr << "Invalid value for -f: '" << ::optarg << "'\n";
	  return 1;
	}
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

  auto format = SudokuFormat(type);
  switch (opt_format) {
    case DEFAULT: break;
    case ONELINE: format = SudokuFormat::oneline(type); break;
    case COMPACT: format = SudokuFormat::compact(type); break;
    case PRESERVE: std::cerr << "Todo!" << '\n'; break;
  }

  for (char c; std::cin.get(c);) {
    input.push_back(c);
    if (!format.is_cell(c)) {
      continue;
    }

    if (++digit_count < type->size()) {
      continue;
    }

    Sudoku sudoku(type, input);
    if (opt_print_initial) {
      std::cout << sudoku.to_string(format) << '\n';
    }

    auto solved = SudokuSolver().solve(sudoku);
    std::cout << solved.to_string(format) << '\n';

    digit_count = 0;
    input.clear();
  }
}
