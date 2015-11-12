#pragma once

#include <string>
#include <vector>

class SudokuType
{
public:
  struct SudokuTooBigException {};

  // The usual 9x9 type with 3x3 regions.
  SudokuType();

  // Sudoku with square regions of any size.
  explicit SudokuType(unsigned region_size);

  // Sudoku with rectangle-shaped regions.
  SudokuType(unsigned region_width, unsigned region_height);

  // Sudoku with arbitrarily-shaped regions.
  explicit SudokuType(std::vector<unsigned> regions);

  void set_alphabet(std::string alphabet);

  bool is_cell(char) const;
  unsigned value(char) const;
  char label(unsigned) const;

  unsigned n() const;
  unsigned size() const;
  unsigned region(unsigned x, unsigned y) const;

private:
  unsigned n_;
  std::string empty_chars_;
  std::string alphabet_;
  std::vector<unsigned> region_;

  static std::vector<unsigned> box_regions(unsigned w, unsigned h);
  static std::string default_alphabet(unsigned n);
  static unsigned isqrt(unsigned n);
};
