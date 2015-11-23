#pragma once

#include <iostream>
#include <vector>

class ExactCoverProblem
{
public:
  explicit ExactCoverProblem(
    unsigned width = 0,
    unsigned secondary_columns = 0
  );

  ExactCoverProblem(
    unsigned width,
    std::vector<std::vector<unsigned>> rows,
    unsigned secondary_columns = 0
  );

  static ExactCoverProblem dense(
    const std::vector<std::vector<unsigned>>& bit_rows,
    unsigned secondary_columns = 0
  );

  auto width() const -> unsigned;
  auto rows() const -> std::vector<std::vector<unsigned>>;
  auto secondary_columns() const -> unsigned;

  void add_row(std::vector<unsigned> row);

private:
  std::vector<std::vector<unsigned>> rows_;
  unsigned width_;
  unsigned secondary_columns_;
};

std::ostream& operator<<(std::ostream&, const ExactCoverProblem&);
