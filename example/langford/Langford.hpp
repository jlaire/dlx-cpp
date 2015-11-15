#pragma once

#include <vector>

class Langford
{
public:
  explicit Langford(unsigned n);

  auto count_solutions() const -> unsigned;
  auto find_solutions() const -> std::vector<std::vector<unsigned>>;

private:
  struct Placement {
    unsigned value;
    unsigned left_pos;
  };

  unsigned n_;
  std::vector<std::vector<unsigned>> rows_;
  std::vector<Placement> row_data_;
};
