#include "../../example/langford/Langford.hpp"

#include <gtest/gtest.h>

namespace {

TEST(Langford_test, count_solutions) {
  EXPECT_EQ(0, Langford(1).count_solutions());
  EXPECT_EQ(0, Langford(2).count_solutions());
  EXPECT_EQ(1, Langford(3).count_solutions());
  EXPECT_EQ(1, Langford(4).count_solutions());
  EXPECT_EQ(0, Langford(5).count_solutions());
  EXPECT_EQ(0, Langford(6).count_solutions());
  EXPECT_EQ(26, Langford(7).count_solutions());
  EXPECT_EQ(150, Langford(8).count_solutions());
  EXPECT_EQ(0, Langford(9).count_solutions());
  EXPECT_EQ(0, Langford(10).count_solutions());
  EXPECT_EQ(17792, Langford(11).count_solutions());
  EXPECT_EQ(108144, Langford(12).count_solutions());
}

TEST(Langford_test, find_solutions) {
  using VV = std::vector<std::vector<unsigned>>;
  EXPECT_EQ(VV(), Langford(1).find_solutions());
  EXPECT_EQ((VV{{3,1,2,1,3,2}}), Langford(3).find_solutions());
}

}
