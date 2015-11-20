#include <dlx/GeneralizedAlgorithmDLX.hpp>

#include <gtest/gtest.h>

namespace {

TEST(GeneralizedAlgorithmDLX_test, test1) {
  auto dlx = GeneralizedAlgorithmDLX(GeneralizedLinkedMatrix::make(4u, {
    {-1, 2},
    {-1, 3},
  }, 2));
  EXPECT_EQ(1u, dlx.count_solutions());
}

TEST(GeneralizedAlgorithmDLX_test, test2) {
  auto dlx = GeneralizedAlgorithmDLX(GeneralizedLinkedMatrix::make(4u, {
    {-1, 2},
    {1, 3},
  }, 2));
  EXPECT_EQ(0u, dlx.count_solutions());
}

TEST(GeneralizedAlgorithmDLX_test, bug) {
  auto dlx = GeneralizedAlgorithmDLX(GeneralizedLinkedMatrix::make(8, {
    {1, -2, 6},
    {-1, 2, 6},
    {3, -5, 6},
    {-1, 4, 7},
    {-1, -2, 5, 7},
  }, 6));
  EXPECT_EQ(2, dlx.count_solutions());
}

}
