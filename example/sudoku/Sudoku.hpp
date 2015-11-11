#pragma once

#include <assert.h>
#include <string>

class Sudoku
{
public:
  static std::string solve(std::string grid);

  static const unsigned N = 9;
  static const unsigned size = N * N;

private:
  static unsigned id_cell(unsigned x, unsigned y) {
    assert(x < N && y < N);
    return x * N + y;
  }

  static unsigned id_col(unsigned x, unsigned n) {
    assert(x < N && n < N);
    return N * N + x * N + n;
  }

  static unsigned id_row(unsigned y, unsigned n) {
    assert(y < N && n < N);
    return 2 * N * N + y * N + n;
  }

  static unsigned id_box(unsigned i, unsigned n) {
    assert(i < N && n < N);
    return 3 * N * N + i * N + n;
  }

  static unsigned get_box(unsigned x, unsigned y) {
    assert(x < N && y < N);
    return y / 3 * 3 + x / 3;
  }
};
