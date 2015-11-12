#pragma once

#include "SudokuType.hpp"
#include <string>
#include <vector>

class SudokuSolver
{
public:
  struct SudokuTooBigException {};

  template <typename... Args>
  explicit SudokuSolver(Args&&... args)
    : type_(std::forward<Args>(args)...)
  {
  }

  std::string solve(std::string grid) const;

private:
  SudokuType type_;

  unsigned pack(unsigned, unsigned) const;
  unsigned id_cell(unsigned x, unsigned y) const;
  unsigned id_col(unsigned x, unsigned d) const;
  unsigned id_row(unsigned y, unsigned d) const;
  unsigned id_region(unsigned i, unsigned d) const;
};
