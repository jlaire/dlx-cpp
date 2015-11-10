#include "Sudoku.hpp"
#include "../include/AlgorithmDLX.hpp"

#include <assert.h>
#include <unordered_map>
#include <vector>

std::string Sudoku::solve(std::string grid) {
  assert(grid.size() == 81);
  for (char c : grid) {
    if (c < '0' || c > '9') {
      return "Invalid sudoku!";
    }
  }
  unsigned col_taken[9][9]{{0}};
  unsigned row_taken[9][9]{{0}};
  unsigned box_taken[9][9]{{0}};
  for (unsigned i = 0; i < 81; ++i) {
    if (grid[i] != '0') {
      unsigned x = i % 9;
      unsigned y = i / 9;
      unsigned n = grid[i] - '1';
      if (++col_taken[x][n] > 1) return "Unsolvable sudoku :(";
      if (++row_taken[y][n] > 1) return "Unsolvable sudoku :(";
      if (++box_taken[get_box(x, y)][n] > 1) return "Unsolvable sudoku :(";
    }
  }

  std::vector<std::vector<unsigned>> matrix;
  for (unsigned i = 0; i < 9; ++i) {
    for (unsigned j = 0; j < 9; ++j) {
      if (col_taken[i][j]) matrix.push_back({id_col(i, j)});
      if (row_taken[i][j]) matrix.push_back({id_row(i, j)});
      if (box_taken[i][j]) matrix.push_back({id_box(i, j)});
    }
  }

  std::unordered_map<unsigned, unsigned> xs, ys, ns;
  for (unsigned y = 0; y < 9; ++y) {
    for (unsigned x = 0; x < 9; ++x) {
      for (unsigned n = 0; n < 9; ++n) {
        if (col_taken[x][n]
            || row_taken[y][n]
            || box_taken[get_box(x, y)][n]
	    || grid[y * 9 + x] != '0')
        {
          continue;
        }
        unsigned row_index = matrix.size();
        xs[row_index] = x;
        ys[row_index] = y;
        ns[row_index] = n;
        matrix.push_back({
          id_cell(x, y),
          id_col(x, n),
          id_row(y, n),
          id_box(get_box(x, y), n)
        });
      }
    }
  }

  std::string solution("No solution!");
  std::function<void(const std::vector<unsigned>& rows)> callback;
  callback = [&](const std::vector<unsigned>& rows) {
    solution = grid;
    for (unsigned i : rows) {
      if (xs.find(i) == xs.end()) {
	continue;
      }
      solution[ys[i] * 9 + xs[i]] = ns[i] + '1';
    }
  };
  auto linked_matrix = LinkedMatrix::from_sparse_matrix(matrix, 9 * 9, 4 * 9 * 9);
  AlgorithmDLX dlx(std::move(linked_matrix), callback);
  dlx.search();
  return solution;
}
