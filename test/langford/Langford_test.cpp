#include "../../example/langford/Langford.hpp"

#include <gtest/gtest.h>

namespace {

TEST(Langford_test, count_solutions) {
  EXPECT_EQ(0, Langford::count_solutions(1));
  EXPECT_EQ(0, Langford::count_solutions(2));
  EXPECT_EQ(1, Langford::count_solutions(3));
  EXPECT_EQ(1, Langford::count_solutions(4));
  EXPECT_EQ(0, Langford::count_solutions(5));
  EXPECT_EQ(0, Langford::count_solutions(6));
  EXPECT_EQ(26, Langford::count_solutions(7));
  EXPECT_EQ(150, Langford::count_solutions(8));
  EXPECT_EQ(0, Langford::count_solutions(9));
  EXPECT_EQ(0, Langford::count_solutions(10));
  EXPECT_EQ(17792, Langford::count_solutions(11));
  EXPECT_EQ(108144, Langford::count_solutions(12));
}

}
