#include "../../example/sudoku/SudokuType.hpp"

#include <gtest/gtest.h>

namespace {

TEST(SudokuType_test, ctor_default) {
  SudokuType type;
  EXPECT_EQ(9, type.n());
  EXPECT_EQ(81, type.size());
}

TEST(SudokuType_test, ctor_nxn_region) {
  EXPECT_EQ(1, SudokuType(1).n());
  EXPECT_EQ(1, SudokuType(1).size());
  EXPECT_EQ(4, SudokuType(4).n());
  EXPECT_EQ(16, SudokuType(4).size());
  EXPECT_EQ(9, SudokuType(9).n());
  EXPECT_EQ(81, SudokuType(9).size());
  EXPECT_EQ(16, SudokuType(16).n());
  EXPECT_EQ(256, SudokuType(16).size());

  // Not squares
  EXPECT_THROW(SudokuType(0), std::invalid_argument);
  EXPECT_THROW(SudokuType(3), std::invalid_argument);
  EXPECT_THROW(SudokuType(5), std::invalid_argument);
  EXPECT_THROW(SudokuType(8), std::invalid_argument);

  // Too big
  EXPECT_THROW(SudokuType(100), std::invalid_argument);
}

TEST(SudokuType_test, ctor_nxm_region) {
  EXPECT_THROW(SudokuType(0, 5), std::invalid_argument);
  EXPECT_THROW(SudokuType(5, 0), std::invalid_argument);
  EXPECT_THROW(SudokuType(1, 100), std::invalid_argument);

  EXPECT_EQ(1, SudokuType(1, 1).n());
  EXPECT_EQ(2, SudokuType(1, 2).n());
  EXPECT_EQ(2, SudokuType(2, 1).n());
  EXPECT_EQ(9, SudokuType(1, 9).n());

  EXPECT_EQ(6, SudokuType(2, 3).n());
  EXPECT_EQ(8, SudokuType(2, 4).n());
  EXPECT_EQ(10, SudokuType(2, 5).n());
}

TEST(SudokuType_test, ctor_custom_region) {
  // size is not a square
  EXPECT_THROW(SudokuType(std::vector<unsigned>()), std::invalid_argument);
  EXPECT_THROW((SudokuType{0, 1}), std::invalid_argument);

  // invalid regions
  EXPECT_THROW((SudokuType{0, 0, 0, 1}), std::invalid_argument);
  EXPECT_THROW((SudokuType{0, 0, 1, 2}), std::invalid_argument);

  EXPECT_NO_THROW(SudokuType{0});
  EXPECT_NO_THROW(SudokuType{1});
  EXPECT_NO_THROW(SudokuType{999});
  EXPECT_NO_THROW((SudokuType{0, 0, 1, 1}));
  EXPECT_NO_THROW((SudokuType{0, 1, 1, 0}));
}

TEST(SudokuType_test, default_labels) {
  SudokuType type(16);

  EXPECT_EQ(true, type.is_cell('.'));
  EXPECT_EQ(true, type.is_cell('0'));
  EXPECT_EQ(false, type.is_cell(' '));

  EXPECT_EQ(true, type.is_cell('1'));
  EXPECT_EQ(true, type.is_cell('2'));
  EXPECT_EQ(true, type.is_cell('3'));
  EXPECT_EQ(true, type.is_cell('4'));
  EXPECT_EQ(true, type.is_cell('5'));
  EXPECT_EQ(true, type.is_cell('6'));
  EXPECT_EQ(true, type.is_cell('7'));
  EXPECT_EQ(true, type.is_cell('8'));
  EXPECT_EQ(true, type.is_cell('9'));
  EXPECT_EQ(true, type.is_cell('A'));
  EXPECT_EQ(true, type.is_cell('B'));
  EXPECT_EQ(true, type.is_cell('C'));
  EXPECT_EQ(true, type.is_cell('D'));
  EXPECT_EQ(true, type.is_cell('E'));
  EXPECT_EQ(true, type.is_cell('F'));
  EXPECT_EQ(true, type.is_cell('G'));

  EXPECT_EQ(0, type.value(' '));
  EXPECT_EQ(0, type.value('.'));
  EXPECT_EQ(0, type.value('0'));
  EXPECT_EQ(1, type.value('1'));
  EXPECT_EQ(2, type.value('2'));
  EXPECT_EQ(3, type.value('3'));
  EXPECT_EQ(4, type.value('4'));
  EXPECT_EQ(5, type.value('5'));
  EXPECT_EQ(6, type.value('6'));
  EXPECT_EQ(7, type.value('7'));
  EXPECT_EQ(8, type.value('8'));
  EXPECT_EQ(9, type.value('9'));
  EXPECT_EQ(10, type.value('A'));
  EXPECT_EQ(11, type.value('B'));
  EXPECT_EQ(12, type.value('C'));
  EXPECT_EQ(13, type.value('D'));
  EXPECT_EQ(14, type.value('E'));
  EXPECT_EQ(15, type.value('F'));
  EXPECT_EQ(16, type.value('G'));

  EXPECT_EQ('1', type.label(0));
  EXPECT_EQ('2', type.label(1));
  EXPECT_EQ('3', type.label(2));
  EXPECT_EQ('4', type.label(3));
  EXPECT_EQ('5', type.label(4));
  EXPECT_EQ('6', type.label(5));
  EXPECT_EQ('7', type.label(6));
  EXPECT_EQ('8', type.label(7));
  EXPECT_EQ('9', type.label(8));
  EXPECT_EQ('A', type.label(9));
  EXPECT_EQ('B', type.label(10));
  EXPECT_EQ('C', type.label(11));
  EXPECT_EQ('D', type.label(12));
  EXPECT_EQ('E', type.label(13));
  EXPECT_EQ('F', type.label(14));
  EXPECT_EQ('G', type.label(15));
  ASSERT_DEATH(type.label(16), "Assertion");
}

TEST(SudokuType_test, custom_labels) {
  SudokuType type(4);

  ASSERT_THROW(type.set_labels("xyz"), std::invalid_argument);
  ASSERT_THROW(type.set_labels("xyzwa"), std::invalid_argument);
  ASSERT_THROW(type.set_labels("xyz0"), std::invalid_argument);
  ASSERT_THROW(type.set_labels("xyz."), std::invalid_argument);
  ASSERT_THROW(type.set_labels("xyzx"), std::invalid_argument);

  EXPECT_EQ(true, type.is_cell('1'));
  EXPECT_EQ(1, type.value('1'));

  type.set_labels("wzyx");

  EXPECT_EQ(false, type.is_cell('1'));
  EXPECT_EQ(0, type.value('1'));

  EXPECT_EQ(true, type.is_cell('y'));
  EXPECT_EQ(1, type.value('w'));
  EXPECT_EQ(4, type.value('x'));
  EXPECT_EQ('z', type.label(1));
}

}
