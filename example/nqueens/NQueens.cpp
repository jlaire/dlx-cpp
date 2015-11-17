#include "NQueens.hpp"

#include <dlx/AlgorithmDLX.hpp>
#include <assert.h>

NQueens::NQueens(unsigned n)
  : n_(n)
{
  assert(n_ > 0);
  auto D = n + n - 1;
  for (auto y = 0u; y < n; ++y) {
    for (auto x = 0u; x < n; ++x) {
      row_data_.push_back({x, y});
      auto d1 = x + y;
      auto d2 = x + n - y - 1;
      rows_.push_back({
        d1,
        D + d2,
        D + D + x,
        D + D + n + y,
      });
    }
  }
}

unsigned NQueens::count_solutions() const {
  auto lm = LinkedMatrix::make(6 * n_ - 2, rows_, 4 * n_ - 2);
  auto dlx = AlgorithmDLX(std::move(lm));
  return dlx.count_solutions();
}

std::vector<std::vector<unsigned>> NQueens::find_solutions() const {
  auto solutions = std::vector<std::vector<unsigned>>();
  auto lm = LinkedMatrix::make(6 * n_ - 2, rows_, 4 * n_ - 2);
  auto dlx = AlgorithmDLX(std::move(lm));
  for (const auto& used_rows : dlx.find_solutions()) {
    auto solution = std::vector<unsigned>(n_);
    for (auto i : used_rows) {
      solution[row_data_[i].y] = row_data_[i].x;
    }
    solutions.emplace_back(std::move(solution));
  }
  return solutions;
}
