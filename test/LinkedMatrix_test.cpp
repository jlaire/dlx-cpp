#include "../include/LinkedMatrix.hpp"

#include <gtest/gtest.h>

namespace {

TEST(LinkedMatrix_test, empty_matrix) {
  auto M = LinkedMatrix::from_boolean_rows({});
  EXPECT_EQ(M->root_id(), M->R(M->root_id()));
}

}
