#include "Langford.hpp"

#include <assert.h>
#include <dlx/AlgorithmDLX.hpp>

unsigned Langford::count_solutions(unsigned n) {
  auto rows = std::vector<std::vector<unsigned>>();
  for (auto size = 1u; size <= n; ++size) {
    for (auto pos = 0u; pos + size + 1 < 2 * n; ++pos) {
      rows.push_back({size - 1, n + pos, n + pos + size + 1});
    }
  }

  auto solution_count = 0u;
  auto lm = LinkedMatrix::from_sparse_matrix(3 * n, rows);
  auto dlx = AlgorithmDLX(std::move(lm), [&](const std::vector<unsigned>& used_rows) -> bool {
    ++solution_count;
    return false;
  });
  dlx.search();

  assert(solution_count % 2 == 0);
  return solution_count / 2;
}
