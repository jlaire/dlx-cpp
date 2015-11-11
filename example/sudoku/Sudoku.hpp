#pragma once

#include <assert.h>
#include <string>
#include <vector>

class Sudoku
{
public:
  struct SudokuTooBigException {};

  // Standard 9x9 sudoku.
  Sudoku();

  // NxN sudoku.
  // In normal sudoku, region_size is 3.
  explicit Sudoku(unsigned region_size);

  // Sudoku with rectangle-shaped regions.
  // In normal sudoku, the regions are 3 by 3.
  Sudoku(unsigned region_width, unsigned region_height);

  std::string solve(std::string grid) const;

  bool is_cell(char) const;
  unsigned value(char) const;
  unsigned size() const;

private:
  unsigned n_;
  std::string empty_chars_;
  std::string alphabet_;
  std::vector<unsigned> region_;

  unsigned pack(unsigned, unsigned) const;
  unsigned id_cell(unsigned x, unsigned y) const;
  unsigned id_col(unsigned x, unsigned d) const;
  unsigned id_row(unsigned y, unsigned d) const;
  unsigned id_region(unsigned i, unsigned d) const;
  unsigned get_region(unsigned x, unsigned y) const;
};
