#pragma once

#include <assert.h>
#include <string>

class Sudoku
{
public:
  static std::string solve(std::string grid);

private:
  static unsigned id_cell(unsigned x, unsigned y) {
    assert(x < 9 && y < 9);
    return x * 9 + y;
  }

  static unsigned id_col(unsigned x, unsigned n) {
    assert(x < 9 && n < 9);
    return 9 * 9 + x * 9 + n;
  }

  static unsigned id_row(unsigned y, unsigned n) {
    assert(y < 9 && n < 9);
    return 2 * 9 * 9 + y * 9 + n;
  }

  static unsigned id_box(unsigned i, unsigned n) {
    assert(i < 9 && n < 9);
    return 3 * 9 * 9 + i * 9 + n;
  }

  static unsigned get_box(unsigned x, unsigned y) {
    assert(x < 9 && y < 9);
    return y / 3 * 3 + x / 3;
  }
};
