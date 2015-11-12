#pragma once

#include <assert.h>
#include <string>
#include <vector>

class SudokuSolver
{
public:
  struct SudokuTooBigException {};

  // Standard 9x9 sudoku.
  SudokuSolver();

  // NxN sudoku.
  // In normal sudoku, region_size is 3.
  explicit SudokuSolver(unsigned region_size);

  // Sudoku with rectangle-shaped regions.
  // In normal sudoku, the regions are 3 by 3.
  SudokuSolver(unsigned region_width, unsigned region_height);

  // Sudoku with arbitrarily shaped regions.
  explicit SudokuSolver(std::vector<unsigned> regions);

  void set_alphabet(std::string alphabet);

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

  static std::vector<unsigned> box_regions(unsigned w, unsigned h);
  static std::string default_alphabet(unsigned n);
  static unsigned isqrt(unsigned size);
};
