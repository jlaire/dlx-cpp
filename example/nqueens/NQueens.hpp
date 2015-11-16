#pragma once

#include <vector>

class NQueens
{
public:
  explicit NQueens(unsigned n);

  auto count_solutions() const -> unsigned;
  auto find_solutions() const -> std::vector<std::vector<unsigned>>;

private:
  struct Position {
    unsigned x;
    unsigned y;
  };

  unsigned n_;
  std::vector<std::vector<unsigned>> rows_;
  std::vector<Position> row_data_;
};
