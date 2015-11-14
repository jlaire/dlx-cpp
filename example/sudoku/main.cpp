#include "SudokuSolver.hpp"
#include "SudokuType.hpp"

#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

int main(int argc, char **argv) {
  enum class Format { DEFAULT, ONELINE, COMPACT, PRESERVE };
  std::map<std::string, Format> format_names{
    {"default", Format::DEFAULT},
    {"oneline", Format::ONELINE},
    {"compact", Format::COMPACT},
    {"preserve", Format::PRESERVE},
  };

  auto opt_format = Format::DEFAULT;
  auto opt_print_initial = false;

  for (int opt; (opt = ::getopt(argc, argv, "if:")) != -1;) {
    switch (opt) {
      case 'i':
	opt_print_initial = true;
	break;

      case 'f': {
	auto it = format_names.find(::optarg);
	if (it == format_names.end()) {
	  std::cerr << "Invalid argument for -f '" << ::optarg << "'\n";
	  return 1;
	}
	opt_format = it->second;
	break;
      }

      default:
	throw std::logic_error("unhandled case in getopt loop");
    }
  }

  std::string input;
  for (std::string line; std::cin;) {
    if (std::getline(std::cin, line) && !line.empty()) {
      input += line;
      input += '\n';
      continue;
    }

    if (input.empty()) {
      continue;
    }

    auto type = SudokuType::guess(input);
    auto format = (
      opt_format == Format::PRESERVE ? SudokuFormat(type, input) :
      opt_format == Format::COMPACT ? SudokuFormat::compact(type) :
      opt_format == Format::ONELINE ? SudokuFormat::oneline(type) :
      SudokuFormat(type)
    );

    Sudoku sudoku(type, input);
    if (opt_print_initial) {
      std::cout << sudoku.to_string(format) << '\n';
    }

    auto solved = SudokuSolver().solve(sudoku);
    std::cout << solved.to_string(format) << '\n';

    input.clear();
  }
}
