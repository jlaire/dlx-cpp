#include "Langford.hpp"

#include <assert.h>
#include <dlx/AlgorithmDLX.hpp>

Langford::Langford(unsigned n)
  : n_(n)
{
  for (auto value = 1u; value <= n_; ++value) {
    for (auto pos = 0u; pos + value + 1 < 2 * n_; ++pos) {
      if (value == 1 && pos + 2 > n) {
        continue;
      }
      row_data_.push_back({value, pos});
      rows_.push_back({value - 1, n_ + pos, n_ + pos + value + 1});
    }
  }
}

unsigned Langford::count_solutions() const {
  auto lm = LinkedMatrix::make(3 * n_, rows_);
  auto dlx = AlgorithmDLX(std::move(lm));
  return dlx.count_solutions();
}

std::vector<std::vector<unsigned>> Langford::find_solutions() const {
  auto solutions = std::vector<std::vector<unsigned>>();
  auto lm = LinkedMatrix::make(3 * n_, rows_);
  auto dlx = AlgorithmDLX(std::move(lm));
  auto solution_rows = dlx.find_solutions();
  for (const auto& used_rows : solution_rows) {
    auto solution = std::vector<unsigned>(2 * n_);
    for (auto i : used_rows) {
      auto pos = row_data_[i].left_pos;
      auto value = row_data_[i].value;
      solution[pos] = value;
      solution[pos + value + 1] = value;
    }
    solutions.emplace_back(std::move(solution));
  }
  return solutions;
}
