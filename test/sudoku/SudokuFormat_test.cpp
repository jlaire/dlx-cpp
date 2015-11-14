#include "../../example/sudoku/SudokuFormat.hpp"

#include <gtest/gtest.h>

namespace {

TEST(SudokuFormat_test, make_default) {
  EXPECT_EQ(
    "+-+\n"
    "|.|\n"
    "+-+\n",
    SudokuFormat::make_default(SudokuType::make(1)).to_string()
  );

  EXPECT_EQ(
    "+--+--+\n"
    "|..|..|\n"
    "|..|..|\n"
    "+--+--+\n"
    "|..|..|\n"
    "|..|..|\n"
    "+--+--+\n",
    SudokuFormat::make_default(SudokuType::make(4)).to_string()
  );

  EXPECT_EQ(
    "+---+---+\n"
    "|...|...|\n"
    "|...|...|\n"
    "+---+---+\n"
    "|...|...|\n"
    "|...|...|\n"
    "+---+---+\n"
    "|...|...|\n"
    "|...|...|\n"
    "+---+---+\n",
    SudokuFormat::make_default(SudokuType::make(3, 2)).to_string()
  );

  auto type = SudokuType::make(std::vector<unsigned>{
    0, 0, 0, 1,
    0, 1, 1, 1,
    2, 2, 3, 3,
    2, 2, 3, 3,
  });
  EXPECT_EQ(
    "+-----+-+\n"
    "|. . .|.|\n"
    "| +---+ |\n"
    "|.|. . .|\n"
    "+-+-+---+\n"
    "|. .|. .|\n"
    "|. .|. .|\n"
    "+---+---+\n",
    SudokuFormat::make_default(type).to_string()
  );

  // In black-and-white ASCII it's hard to do better than this.
  auto type2 = SudokuType::make(std::vector<unsigned>{
    0, 1,
    1, 0,
  });
  EXPECT_EQ(
    "+-+-+\n"
    "|.|.|\n"
    "+-+-+\n"
    "|.|.|\n"
    "+-+-+\n",
    SudokuFormat::make_default(type2).to_string()
  );
}

TEST(SudokuFormat_test, make_compact) {
  EXPECT_EQ(
    ".\n",
    SudokuFormat::make_compact(SudokuType::make(1)).to_string()
  );

  EXPECT_EQ(
    "....\n"
    "....\n"
    "....\n"
    "....\n",
    SudokuFormat::make_compact(SudokuType::make(4)).to_string()
  );
}

TEST(SudokuFormat_test, make_oneline) {
  EXPECT_EQ(
    ".",
    SudokuFormat::make_oneline(SudokuType::make(1)).to_string()
  );

  EXPECT_EQ(
    "................",
    SudokuFormat::make_oneline(SudokuType::make(4)).to_string()
  );
}

TEST(SudokuFormat_test, parse) {
  EXPECT_ANY_THROW(SudokuFormat(" "));
  EXPECT_ANY_THROW(SudokuFormat("..."));
  EXPECT_NO_THROW(SudokuFormat("."));
  EXPECT_ANY_THROW(SudokuFormat(SudokuType::make(9), "."));

  EXPECT_EQ(
    "..|..\n"
    "..|..\n"
    "-----\n"
    "..|..\n"
    "..|..\n",
    SudokuFormat(
      "1.|..\n"
      "..|.2\n"
      "-----\n"
      ".1|..\n"
      "..|3.\n"
    ).to_string()
  );
}

TEST(SudokuFormat_test, get_values) {
  auto type = SudokuType::make(4);
  auto format = SudokuFormat::make_default(type);

  EXPECT_ANY_THROW(format.get_values(".... .... .... ..."));
  EXPECT_ANY_THROW(format.get_values(".... .... .... .... ."));

  EXPECT_NO_THROW(format.get_values(".... .... .... ...."));
  EXPECT_NO_THROW(format.get_values("1234 1342 00.. ...."));

  EXPECT_ANY_THROW(format.get_values("5... .... .... ...."));
  EXPECT_ANY_THROW(format.get_values("A... .... .... ...."));
  auto xyzw_format = format.with_labels("XYZW");
  EXPECT_ANY_THROW(xyzw_format.get_values("1... .... .... ...."));
  EXPECT_NO_THROW(xyzw_format.get_values("Z... .... .... ...."));

  EXPECT_EQ(
    std::vector<unsigned>({
      1, 0, 0, 0,
      0, 2, 0, 0,
      0, 0, 3, 0,
      0, 0, 0, 4,
    }),
    format.get_values(
      "1.|.."
      ".2|.."
      "-----"
      "..|3."
      "00|.4"
    )
  );
}

TEST(SudokuFormat_test, to_string) {
  auto format = SudokuFormat::make_default(SudokuType::make(4));
  EXPECT_EQ(
    "+--+--+\n"
    "|..|.4|\n"
    "|2.|..|\n"
    "+--+--+\n"
    "|.1|..|\n"
    "|..|1.|\n"
    "+--+--+\n",
    format.to_string(std::vector<unsigned>{
      0, 0, 0, 4,
      2, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
    })
  );

  format = format.with_labels("ABCD");
  EXPECT_EQ(
    "+--+--+\n"
    "|..|.D|\n"
    "|B.|..|\n"
    "+--+--+\n"
    "|.A|..|\n"
    "|..|A.|\n"
    "+--+--+\n",
    format.to_string(std::vector<unsigned>{
      0, 0, 0, 4,
      2, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
    })
  );
}

TEST(SudokuFormat_test, default_labels) {
  auto format = SudokuFormat::make_default(SudokuType::make(16));

  EXPECT_EQ(true, format.is_cell('.'));
  EXPECT_EQ(true, format.is_cell('0'));
  EXPECT_EQ(false, format.is_cell(' '));

  EXPECT_EQ(true, format.is_cell('1'));
  EXPECT_EQ(true, format.is_cell('2'));
  EXPECT_EQ(true, format.is_cell('3'));
  EXPECT_EQ(true, format.is_cell('4'));
  EXPECT_EQ(true, format.is_cell('5'));
  EXPECT_EQ(true, format.is_cell('6'));
  EXPECT_EQ(true, format.is_cell('7'));
  EXPECT_EQ(true, format.is_cell('8'));
  EXPECT_EQ(true, format.is_cell('9'));
  EXPECT_EQ(true, format.is_cell('A'));
  EXPECT_EQ(true, format.is_cell('B'));
  EXPECT_EQ(true, format.is_cell('C'));
  EXPECT_EQ(true, format.is_cell('D'));
  EXPECT_EQ(true, format.is_cell('E'));
  EXPECT_EQ(true, format.is_cell('F'));
  EXPECT_EQ(true, format.is_cell('G'));

  EXPECT_EQ(0, format.value(' '));
  EXPECT_EQ(0, format.value('.'));
  EXPECT_EQ(0, format.value('0'));
  EXPECT_EQ(1, format.value('1'));
  EXPECT_EQ(2, format.value('2'));
  EXPECT_EQ(3, format.value('3'));
  EXPECT_EQ(4, format.value('4'));
  EXPECT_EQ(5, format.value('5'));
  EXPECT_EQ(6, format.value('6'));
  EXPECT_EQ(7, format.value('7'));
  EXPECT_EQ(8, format.value('8'));
  EXPECT_EQ(9, format.value('9'));
  EXPECT_EQ(10, format.value('A'));
  EXPECT_EQ(11, format.value('B'));
  EXPECT_EQ(12, format.value('C'));
  EXPECT_EQ(13, format.value('D'));
  EXPECT_EQ(14, format.value('E'));
  EXPECT_EQ(15, format.value('F'));
  EXPECT_EQ(16, format.value('G'));

  EXPECT_EQ('.', format.label(0));
  EXPECT_EQ('1', format.label(1));
  EXPECT_EQ('2', format.label(2));
  EXPECT_EQ('3', format.label(3));
  EXPECT_EQ('4', format.label(4));
  EXPECT_EQ('5', format.label(5));
  EXPECT_EQ('6', format.label(6));
  EXPECT_EQ('7', format.label(7));
  EXPECT_EQ('8', format.label(8));
  EXPECT_EQ('9', format.label(9));
  EXPECT_EQ('A', format.label(10));
  EXPECT_EQ('B', format.label(11));
  EXPECT_EQ('C', format.label(12));
  EXPECT_EQ('D', format.label(13));
  EXPECT_EQ('E', format.label(14));
  EXPECT_EQ('F', format.label(15));
  EXPECT_EQ('G', format.label(16));
  ASSERT_DEATH(format.label(17), "Assertion");
}

TEST(SudokuFormat_test, with_labels) {
  auto format = SudokuFormat::make_default(SudokuType::make(4));

  ASSERT_THROW(format.with_labels("xyz"), std::invalid_argument);
  ASSERT_THROW(format.with_labels("xyzwa"), std::invalid_argument);
  ASSERT_THROW(format.with_labels("xyz0"), std::invalid_argument);
  ASSERT_THROW(format.with_labels("xyz."), std::invalid_argument);
  ASSERT_THROW(format.with_labels("xyzx"), std::invalid_argument);

  EXPECT_TRUE(format.is_cell('1'));
  EXPECT_FALSE(format.is_cell('x'));
  EXPECT_EQ(1, format.value('1'));

  format = format.with_labels("wzyx");

  EXPECT_FALSE(format.is_cell('1'));
  EXPECT_EQ(0, format.value('1'));

  EXPECT_TRUE(format.is_cell('y'));
  EXPECT_EQ(1, format.value('w'));
  EXPECT_EQ(4, format.value('x'));
  EXPECT_EQ('z', format.label(2));
}

}
