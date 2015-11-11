#include "Sudoku.hpp"

#include <dlx/AlgorithmDLX.hpp>

#include <ctype.h>
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
 * was almost identical. The current implementation makes all columns primary,
 * so it is a proper exact cover.
 */

std::string Sudoku::solve(std::string grid) {
  std::vector<int> digits;
  std::vector<int> digit_positions;
  for (unsigned i = 0; i < grid.size(); ++i) {
    if (::isdigit(grid[i]) || grid[i] == '.') {
      digits.push_back(grid[i] == '.' ? 0 : grid[i] - '0');
      digit_positions.push_back(i);
    }
  }

  if (digits.size() < size) {
    return "Invalid sudoku! Not enough digits.";
  }
  if (digits.size() > size) {
    return "Invalid sudoku! Too many digits.";
  }

  unsigned cell_taken[N][N]{{0}};
  unsigned col_taken[N][N]{{0}};
  unsigned row_taken[N][N]{{0}};
  unsigned box_taken[N][N]{{0}};
  for (unsigned i = 0; i < size; ++i) {
    if (digits[i] != 0) {
      unsigned x = i % N;
      unsigned y = i / N;
      unsigned n = digits[i] - 1;
      if (++cell_taken[x][y] > 1) return "Unsolvable sudoku :(";
      if (++col_taken[x][n] > 1) return "Unsolvable sudoku :(";
      if (++row_taken[y][n] > 1) return "Unsolvable sudoku :(";
      if (++box_taken[get_box(x, y)][n] > 1) return "Unsolvable sudoku :(";
    }
  }

  std::vector<std::vector<unsigned>> matrix;
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < N; ++j) {
      if (cell_taken[i][j]) matrix.push_back({id_cell(i, j)});
      if (col_taken[i][j]) matrix.push_back({id_col(i, j)});
      if (row_taken[i][j]) matrix.push_back({id_row(i, j)});
      if (box_taken[i][j]) matrix.push_back({id_box(i, j)});
    }
  }

  std::unordered_map<unsigned, unsigned> xs, ys, ns;
  for (unsigned y = 0; y < N; ++y) {
    for (unsigned x = 0; x < N; ++x) {
      for (unsigned n = 0; n < N; ++n) {
        if (cell_taken[x][y]
            || col_taken[x][n]
            || row_taken[y][n]
            || box_taken[get_box(x, y)][n]
            || digits[y * N + x] != 0)
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
  std::function<bool(const std::vector<unsigned>& rows)> callback;
  callback = [&](const std::vector<unsigned>& rows) -> bool {
    solution = grid;
    for (unsigned i : rows) {
      if (xs.find(i) == xs.end()) {
        continue;
      }
      solution[digit_positions[ys[i] * N + xs[i]]] = ns[i] + '1';
    }
    return true;
  };
  auto linked_matrix = LinkedMatrix::from_sparse_matrix(4 * N * N, matrix);
  AlgorithmDLX dlx(std::move(linked_matrix), callback);
  dlx.search();
  return solution;
}
