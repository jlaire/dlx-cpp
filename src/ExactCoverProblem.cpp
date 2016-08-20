#include <dlx/ExactCoverProblem.hpp>

#include <algorithm>
#include <stdexcept>

ExactCoverProblem::ExactCoverProblem(unsigned width, unsigned secondary_columns)
  : ExactCoverProblem(width, {}, secondary_columns)
{
}

ExactCoverProblem::ExactCoverProblem(
  unsigned width,
  std::vector<std::vector<unsigned>> rows,
  unsigned secondary_columns
)
  : rows_(),
  width_(width),
  secondary_columns_(secondary_columns)
{
  if (secondary_columns_ > width_) {
    throw std::invalid_argument("secondary_columns > width");
  }
  for (auto&& row : rows) {
    add_row(std::move(row));
  }
}

ExactCoverProblem ExactCoverProblem::dense(
  const std::vector<std::vector<unsigned>>& bit_rows,
  unsigned secondary_columns)
{
  if (bit_rows.empty()) {
    return ExactCoverProblem(0, secondary_columns);
  }

  auto width = bit_rows[0].size();
  auto problem = ExactCoverProblem(width, secondary_columns);

  for (const auto& bits : bit_rows) {
    if (bits.size() != width) {
      throw std::invalid_argument("rows have different lengths");
    }
    auto row = std::vector<unsigned>();
    for (auto i = 0u; i < bits.size(); ++i) {
      if (bits[i] != 0 && bits[i] != 1) {
        throw std::invalid_argument("dense matrix must contain only 0s and 1s");
      }
      if (bits[i]) {
        row.push_back(i);
      }
    }
    problem.add_row(std::move(row));
  }

  return problem;
}

auto ExactCoverProblem::width() const -> unsigned {
  return width_;
}

auto ExactCoverProblem::rows() const -> const std::vector<std::vector<unsigned>>& {
  return rows_;
}

auto ExactCoverProblem::secondary_columns() const -> unsigned {
  return secondary_columns_;
}

void ExactCoverProblem::add_row(std::vector<unsigned> row) {
  std::sort(row.begin(), row.end());
  for (auto x : row) {
    if (x >= width_) {
      throw std::invalid_argument("column out of range");
    }
  }
  for (auto i = 1u; i < row.size(); ++i) {
    if (row[i - 1] == row[i]) {
      throw std::invalid_argument("duplicate columns");
    }
  }
  rows_.emplace_back(std::move(row));
}

std::ostream& operator<<(std::ostream& os, const ExactCoverProblem& problem) {
  os << problem.width() << ' ' << problem.secondary_columns() << '\n';
  for (const auto& row : problem.rows()) {
    for (auto i = 0u; i < row.size(); ++i) {
      os << row[i] << " \n"[i + 1 == row.size()];
    }
  }
  return os;
}
