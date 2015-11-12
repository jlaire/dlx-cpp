#include "SudokuSolver.hpp"

#include <dlx/AlgorithmDLX.hpp>

#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <vector>

/*
 * One choice needs to be made when creating the exact cover problem.
 *
 * The column representing the constraint "there is a number in cell (x,y)" can
 * be either primary or secondary. All cells will be naturally filled if all
 * of the other conditions are met.
 *
 * I tried both options, and the performance on a large set of 17-clue Sudokus
 * was almost identical. So I arbitrarily decided to make all columns primary;
 * that's how most Sudoku exact cover articles seem to do it, and it means we
 * have a proper exact cover.
 */

std::string SudokuSolver::solve(std::string grid) const {
  std::vector<unsigned> digits;
  std::vector<unsigned> digit_positions;
  for (unsigned i = 0; i < grid.size(); ++i) {
    if (type_.is_cell(grid[i])) {
      digits.push_back(type_.value(grid[i]));
      digit_positions.push_back(i);
    }
  }

  if (digits.size() < type_.size()) {
    return "Invalid sudoku! Not enough digits.";
  }
  if (digits.size() > type_.size()) {
    return "Invalid sudoku! Too many digits.";
  }

  std::vector<unsigned> cell_taken(type_.size());
  std::vector<unsigned> col_taken(type_.size());
  std::vector<unsigned> row_taken(type_.size());
  std::vector<unsigned> region_taken(type_.size());
  for (unsigned i = 0; i < type_.size(); ++i) {
    if (digits[i] != 0) {
      unsigned x = i % type_.n();
      unsigned y = i / type_.n();
      unsigned d = digits[i] - 1;
      if (++cell_taken[pack(x, y)] > 1) return "Unsolvable sudoku :(";
      if (++col_taken[pack(x, d)] > 1) return "Unsolvable sudoku :(";
      if (++row_taken[pack(y, d)] > 1) return "Unsolvable sudoku :(";
      if (++region_taken[pack(type_.region(x, y), d)] > 1) return "Unsolvable sudoku :(";
    }
  }

  std::vector<std::vector<unsigned>> matrix;
  for (unsigned i = 0; i < type_.n(); ++i) {
    for (unsigned j = 0; j < type_.n(); ++j) {
      if (cell_taken[pack(i, j)]) matrix.push_back({id_cell(i, j)});
      if (col_taken[pack(i, j)]) matrix.push_back({id_col(i, j)});
      if (row_taken[pack(i, j)]) matrix.push_back({id_row(i, j)});
      if (region_taken[pack(i, j)]) matrix.push_back({id_region(i, j)});
    }
  }

  std::unordered_map<unsigned, unsigned> xs, ys, ds;
  for (unsigned y = 0; y < type_.n(); ++y) {
    for (unsigned x = 0; x < type_.n(); ++x) {
      for (unsigned d = 0; d < type_.n(); ++d) {
        if (cell_taken[pack(x, y)]
            || col_taken[pack(x, d)]
            || row_taken[pack(y, d)]
            || region_taken[pack(type_.region(x, y), d)])
        {
          continue;
        }
        unsigned row_index = matrix.size();
        xs[row_index] = x;
        ys[row_index] = y;
        ds[row_index] = d;
        matrix.push_back({
          id_cell(x, y),
          id_col(x, d),
          id_row(y, d),
          id_region(type_.region(x, y), d)
        });
      }
    }
  }

  std::string solution("No solution!");
  std::function<bool(const std::vector<unsigned>& rows)> callback;
  callback = [&](const std::vector<unsigned>& rows) -> bool {
    solution = grid;
    for (unsigned i : rows) {
      if (xs.find(i) == xs.end()) {
        continue;
      }
      solution[digit_positions[pack(ys[i], xs[i])]] = type_.label(ds[i]);
    }
    return true;
  };
  auto linked_matrix = LinkedMatrix::from_sparse_matrix(4 * type_.size(), matrix);
  AlgorithmDLX dlx(std::move(linked_matrix), callback);
  dlx.search();
  return solution;
}

unsigned SudokuSolver::pack(unsigned a, unsigned b) const {
  assert(a < type_.n() && b < type_.n());
  return a * type_.n() + b;
}

unsigned SudokuSolver::id_cell(unsigned x, unsigned y) const {
  return pack(x, y);
}

unsigned SudokuSolver::id_col(unsigned x, unsigned d) const {
  return type_.size() + pack(x, d);
}

unsigned SudokuSolver::id_row(unsigned y, unsigned d) const {
  return 2 * type_.size() + pack(y, d);
}

unsigned SudokuSolver::id_region(unsigned i, unsigned d) const {
  return 3 * type_.size() + pack(i, d);
}
