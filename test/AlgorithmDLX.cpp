#include <dlx/AlgorithmDLX.hpp>

#include <gtest/gtest.h>

namespace {

TEST(AlgorithmDLX_test, no_rows) {
  auto rows = std::vector<std::vector<unsigned>>();
  auto lm = LinkedMatrix::from_sparse_matrix(1u, rows);
  auto solution_count = 0u;
  auto dlx = AlgorithmDLX(std::move(lm), [&](const std::vector<unsigned>& used_rows) -> bool {
    ++solution_count;
    return false;
  });
  EXPECT_EQ(0u, solution_count);
}

}
