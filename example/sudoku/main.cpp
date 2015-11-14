#include "SudokuSolver.hpp"
#include "SudokuType.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

static void show_usage_and_exit() {
  std::cout << "Help message goes here\n";
  ::exit(1);
}

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
  auto opt_one_sudoku_per_line = false;

  for (int opt; (opt = ::getopt(argc, argv, "hilf:")) != -1;) {
    switch (opt) {
      case 'h':
	show_usage_and_exit();
	break;

      case 'i':
	opt_print_initial = true;
	break;

      case 'l':
	opt_one_sudoku_per_line = true;
	break;

      case 'f': {
	auto it = format_names.find(::optarg);
	if (it == format_names.end()) {
	  std::cerr << "Invalid argument for -f '" << ::optarg << "'\n";
	  show_usage_and_exit();
	}
	opt_format = it->second;
	break;
      }

      default:
	throw std::logic_error("unhandled case in getopt loop");
    }
  }

  std::string input;
  bool first = true;
  for (std::string line; std::cin; first = false) {
    if (std::getline(std::cin, line) && !line.empty()) {
      input += line;
      if (!opt_one_sudoku_per_line) {
	input += '\n';
	continue;
      }
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

    if (!first && opt_format != Format::ONELINE) {
      std::cout << '\n';
    }

    Sudoku sudoku(type, input);
    if (opt_print_initial) {
      std::cout << sudoku.to_string(format);
      if (opt_format != Format::ONELINE) {
	std::cout << '\n';
      }
    }

    auto solved = SudokuSolver().solve(sudoku);
    std::cout << solved.to_string(format);

    input.clear();
  }
}
