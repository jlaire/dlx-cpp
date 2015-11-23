#pragma once

#include "Shape.hpp"
#include <dlx/ExactCoverProblem.hpp>
#include <string>
#include <vector>

class Polyomino
{
public:
  Polyomino();

  auto problem() const -> const ExactCoverProblem&;
  auto make_solution(const std::vector<unsigned>& used_rows) const -> std::vector<std::string>;

private:
  unsigned width_;
  unsigned height_;
  std::vector<std::vector<Shape>> shapes_;
  
  struct RowData
  {
    unsigned shape;
    unsigned variation;
    unsigned x;
    unsigned y;
  };

  std::vector<RowData> row_data_;
  ExactCoverProblem problem_;
};
