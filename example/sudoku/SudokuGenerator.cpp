#include "SudokuGenerator.hpp"
#include "SudokuSolver.hpp"

#include <algorithm>
#include <string>

SudokuGenerator::SudokuGenerator()
  : engine_()
{
  std::random_device rd;
  engine_ = std::mt19937(rd());
}

Sudoku SudokuGenerator::operator()(std::shared_ptr<SudokuType> type) {
  // It might be faster to start with solved Sudoku and remove digits, instead
  // of starting with an empty grid and adding them.
  //
  // But for that we need a way to generate solved Sudokus of any type.

  auto randint = std::uniform_int_distribution<unsigned>(0, type->n() - 1);
  auto sudoku = Sudoku(type);

  for (;;) {
    auto y = randint(engine_);
    auto x = randint(engine_);
    auto d = 1 + randint(engine_);
    auto yx = y * type->n() + x;
    auto prev = sudoku[yx];
    sudoku[yx] = d;
    if (!sudoku.is_valid()) {
      sudoku[yx] = prev;
      continue;
    }

    auto count = count_solutions(sudoku);
    if (count == 0) {
      sudoku[yx] = prev;
      continue;
    }

    if (count != 1) {
      continue;
    }

    auto yxs = std::vector<unsigned>();
    for (auto i = 0u; i < type->size(); ++i) {
      yxs.push_back(i);
    }
    std::shuffle(yxs.begin(), yxs.end(), engine_);
    for (auto yx : yxs) {
      auto d = sudoku[yx];
      if (d != 0) {
        sudoku[yx] = 0;
        if (count_solutions(sudoku) != 1) {
          sudoku[yx] = d;
        }
      }
    }
    return sudoku;
  }
}

unsigned SudokuGenerator::count_solutions(const Sudoku& sudoku) {
  try {
    auto solved = SudokuSolver().solve(sudoku);
    return 1;
  }
  catch (const std::exception& ex) {
    // TODO: Different types of exceptions.
    if (ex.what() == std::string("No solution")) {
      return 0;
    }
    if (ex.what() == std::string("Multiple solutions")) {
      return 2;
    }
    throw;
  }
}
