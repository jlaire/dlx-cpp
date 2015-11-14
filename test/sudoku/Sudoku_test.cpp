#include "../../example/sudoku/Sudoku.hpp"

#include <gtest/gtest.h>

namespace {

TEST(Sudoku_test, default_template_rectangle_regions) {
  EXPECT_EQ(
    "+-+\n"
    "|.|\n"
    "+-+\n",
    Sudoku(std::make_shared<SudokuType>(1)).default_template()
  );

  EXPECT_EQ(
    "+--+--+\n"
    "|..|..|\n"
    "|..|..|\n"
    "+--+--+\n"
    "|..|..|\n"
    "|..|..|\n"
    "+--+--+\n",
    Sudoku(std::make_shared<SudokuType>(4)).default_template()
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
    Sudoku(std::make_shared<SudokuType>(3, 2)).default_template()
  );
}

TEST(Sudoku_test, default_template_arbitrary_regions) {
  auto type = std::make_shared<SudokuType>(std::vector<unsigned>{
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
    Sudoku(type).default_template()
  );

  // In black-and-white ASCII it's hard to do better than this.
  auto type2 = std::make_shared<SudokuType>(std::vector<unsigned>{
    0, 1,
    1, 0,
  });
  EXPECT_EQ(
    "+-+-+\n"
    "|.|.|\n"
    "+-+-+\n"
    "|.|.|\n"
    "+-+-+\n",
    Sudoku(type2).default_template()
  );
}

TEST(Sudoku_test, set_template) {
  Sudoku sudoku(std::make_shared<SudokuType>(4));
  EXPECT_THROW(
    sudoku.set_template(
      "...."
      "...."
      "...."
      "... "
    ),
    std::invalid_argument
  );
  EXPECT_THROW(
    sudoku.set_template(
      "...."
      "...."
      "...."
      "....."
    ),
    std::invalid_argument
  );
  ASSERT_NO_THROW(
    sudoku.set_template(
      "..|..\n"
      "..|..\n"
      "--|--\n"
      "..|..\n"
      "..|..\n"
    )
  );

  EXPECT_EQ(
    "..|..\n"
    "..|..\n"
    "--|--\n"
    "..|..\n"
    "..|..\n",
    sudoku.to_string()
  );
  sudoku[0] = 0;
  sudoku[3] = 4;
  sudoku[4] = 2;
  sudoku[9] = 1;
  sudoku[14] = 1;

  EXPECT_EQ(
    "..|.4\n"
    "2.|..\n"
    "--|--\n"
    ".1|..\n"
    "..|1.\n",
    sudoku.to_string()
  );
}

TEST(Sudoku_test, from_string) {
  EXPECT_THROW(Sudoku(""), std::invalid_argument);
  EXPECT_THROW(Sudoku("2"), std::invalid_argument);
  EXPECT_THROW(Sudoku("A"), std::invalid_argument);
  EXPECT_THROW(Sudoku("z"), std::invalid_argument);
  EXPECT_NO_THROW(Sudoku("."));
  EXPECT_NO_THROW(Sudoku("0"));
  EXPECT_NO_THROW(Sudoku("1"));
  EXPECT_NO_THROW(Sudoku("( 1 )"));

  Sudoku sudoku(
    "1.|.."
    "..|.2"
    "-----"
    ".1|.."
    "..|3."
  );

  EXPECT_EQ(1, sudoku[0]);
  EXPECT_EQ(0, sudoku[1]);
  EXPECT_EQ(0, sudoku[2]);
  EXPECT_EQ(0, sudoku[3]);
  EXPECT_EQ(0, sudoku[4]);
  EXPECT_EQ(0, sudoku[5]);
  EXPECT_EQ(0, sudoku[6]);
  EXPECT_EQ(2, sudoku[7]);
  EXPECT_EQ(0, sudoku[8]);
  EXPECT_EQ(1, sudoku[9]);
  EXPECT_EQ(0, sudoku[10]);
  EXPECT_EQ(0, sudoku[11]);
  EXPECT_EQ(0, sudoku[12]);
  EXPECT_EQ(0, sudoku[13]);
  EXPECT_EQ(3, sudoku[14]);
  EXPECT_EQ(0, sudoku[15]);
}

TEST(Sudoku_test, is_valid) {
  EXPECT_FALSE(
    Sudoku(
      "1.|1."
      "..|.."
      "-----"
      "..|.."
      "..|.."
    ).is_valid()
  );

  EXPECT_FALSE(
    Sudoku(
      "1.|.."
      "..|.."
      "-----"
      "1.|.."
      "..|.."
    ).is_valid()
  );

  EXPECT_FALSE(
    Sudoku(
      "1.|.."
      ".1|.."
      "-----"
      "..|.."
      "..|.."
    ).is_valid()
  );

  EXPECT_TRUE(
    Sudoku(
      "1.|.."
      "..|1."
      "-----"
      ".1|.."
      "..|.1"
    ).is_valid()
  );
}

TEST(Sudoku_test, is_solved) {
  EXPECT_FALSE(Sudoku(".").is_solved());
  EXPECT_TRUE(Sudoku("1").is_solved());
  EXPECT_FALSE(
    Sudoku(
      "12|3."
      "34|12"
      "-----"
      "43|21"
      "21|43"
    ).is_solved()
  );
  EXPECT_TRUE(
    Sudoku(
      "12|34"
      "34|12"
      "-----"
      "43|21"
      "21|43"
    ).is_solved()
  );
}

}
