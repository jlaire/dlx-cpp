#pragma once

#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

class SudokuType
{
public:
  // The standard 9x9 Sudoku.
  SudokuType();

  // NxN version of the 9x9.
  // `region_size` must be a square.
  explicit SudokuType(unsigned size);

  // Sudoku with rectangle-shaped regions.
  SudokuType(unsigned region_width, unsigned region_height);

  // Sudoku with arbitrarily-shaped regions.
  SudokuType(std::initializer_list<unsigned> regions);
  explicit SudokuType(std::vector<unsigned> regions);

  void set_labels(std::string labels);

  // Returns true iff c is a label or represents an empty cell.
  bool is_cell(char c) const;

  // Returns a value in [1, n] if c is a label; 0 otherwise.
  unsigned value(char c) const;
  char label(unsigned) const;

  unsigned n() const;
  unsigned size() const;
  unsigned region(unsigned x, unsigned y) const;

private:
  unsigned n_;
  std::string empty_chars_;
  std::string labels_;
  std::vector<unsigned> region_;

  static std::vector<unsigned> box_regions(unsigned w, unsigned h);
  static std::vector<unsigned> normalize_regions(std::vector<unsigned> regions);
  static std::string default_labels(unsigned n);
  static unsigned isqrt(unsigned n);
};
