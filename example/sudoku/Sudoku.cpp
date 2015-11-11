#include "Sudoku.hpp"

#include <dlx/AlgorithmDLX.hpp>

#include <algorithm>
#include <unordered_map>
#include <vector>

Sudoku::Sudoku()
  : Sudoku(3)
{
}

Sudoku::Sudoku(unsigned box_size)
  : Sudoku(box_size, box_size)
{
}

Sudoku::Sudoku(unsigned region_width, unsigned region_height)
  : rw_(region_width),
  rh_(region_height),
  n_(rw_ * rh_)
{
  static const std::string chars(
    "123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
  );

  if (n_ > chars.size()) {
    throw SudokuTooBigException();
  }

  empty_chars_ = {'.', '0'};

  for (unsigned i = 0; i < n_; ++i) {
    alphabet_.push_back(chars[i]);
  }

  region_.resize(size());
  unsigned regions_per_row = n_ / rw_;
  for (unsigned y = 0; y < n_; ++y) {
    for (unsigned x = 0; x < n_; ++x) {
      region_[pack(y, x)] = y / rh_ * regions_per_row + x / rw_;
    }
  }
}

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

std::string Sudoku::solve(std::string grid) const {
  std::vector<unsigned> digits;
  std::vector<unsigned> digit_positions;
  for (unsigned i = 0; i < grid.size(); ++i) {
    if (is_cell(grid[i])) {
      digits.push_back(value(grid[i]));
      digit_positions.push_back(i);
    }
  }

  if (digits.size() < size()) {
    return "Invalid sudoku! Not enough digits.";
  }
  if (digits.size() > size()) {
    return "Invalid sudoku! Too many digits.";
  }

  std::vector<unsigned> cell_taken(size());
  std::vector<unsigned> col_taken(size());
  std::vector<unsigned> row_taken(size());
  std::vector<unsigned> region_taken(size());
  for (unsigned i = 0; i < size(); ++i) {
    if (digits[i] != 0) {
      unsigned x = i % n_;
      unsigned y = i / n_;
      unsigned d = digits[i] - 1;
      if (++cell_taken[pack(x, y)] > 1) return "Unsolvable sudoku :(";
      if (++col_taken[pack(x, d)] > 1) return "Unsolvable sudoku :(";
      if (++row_taken[pack(y, d)] > 1) return "Unsolvable sudoku :(";
      if (++region_taken[pack(get_region(x, y), d)] > 1) return "Unsolvable sudoku :(";
    }
  }

  std::vector<std::vector<unsigned>> matrix;
  for (unsigned i = 0; i < n_; ++i) {
    for (unsigned j = 0; j < n_; ++j) {
      if (cell_taken[pack(i, j)]) matrix.push_back({id_cell(i, j)});
      if (col_taken[pack(i, j)]) matrix.push_back({id_col(i, j)});
      if (row_taken[pack(i, j)]) matrix.push_back({id_row(i, j)});
      if (region_taken[pack(i, j)]) matrix.push_back({id_region(i, j)});
    }
  }

  std::unordered_map<unsigned, unsigned> xs, ys, ds;
  for (unsigned y = 0; y < n_; ++y) {
    for (unsigned x = 0; x < n_; ++x) {
      for (unsigned d = 0; d < n_; ++d) {
        if (cell_taken[pack(x, y)]
            || col_taken[pack(x, d)]
            || row_taken[pack(y, d)]
            || region_taken[pack(get_region(x, y), d)])
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
          id_region(get_region(x, y), d)
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
      solution[digit_positions[pack(ys[i], xs[i])]] = alphabet_[ds[i]];
    }
    return true;
  };
  auto linked_matrix = LinkedMatrix::from_sparse_matrix(4 * size(), matrix);
  AlgorithmDLX dlx(std::move(linked_matrix), callback);
  dlx.search();
  return solution;
}

bool Sudoku::is_cell(char c) const {
  return empty_chars_.find(c) != std::string::npos
    || alphabet_.find(c) != std::string::npos;
}

unsigned Sudoku::value(char c) const {
  auto pos = alphabet_.find(c);
  return pos == std::string::npos ? 0 : pos + 1;
}

unsigned Sudoku::size() const {
  return n_ * n_;
}

unsigned Sudoku::pack(unsigned a, unsigned b) const {
  assert(a < n_ && b < n_);
  return a * n_ + b;
}

unsigned Sudoku::id_cell(unsigned x, unsigned y) const {
  return pack(x, y);
}

unsigned Sudoku::id_col(unsigned x, unsigned d) const {
  return size() + pack(x, d);
}

unsigned Sudoku::id_row(unsigned y, unsigned d) const {
  return 2 * size() + pack(y, d);
}

unsigned Sudoku::id_region(unsigned i, unsigned d) const {
  return 3 * size() + pack(i, d);
}

unsigned Sudoku::get_region(unsigned x, unsigned y) const {
  return region_[pack(y, x)];
}
