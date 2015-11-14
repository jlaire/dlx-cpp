#include "../example/sudoku/SudokuSolver.hpp"
#include "../example/sudoku/Sudoku.hpp"
#include "../example/sudoku/SudokuFormat.hpp"
#include "../example/sudoku/SudokuType.hpp"

#include <gtest/gtest.h>

namespace {

TEST(SudokuSolver_test, already_solved) {
  Sudoku sudoku("846937152319625847752184963285713694463859271971246385127598436638471529594362718");
  ASSERT_TRUE(sudoku.is_solved());
  EXPECT_EQ(sudoku, SudokuSolver().solve(sudoku));
}

TEST(SudokuSolver_test, invalid) {
  Sudoku sudoku(std::string(81, '1'));
  ASSERT_FALSE(sudoku.is_valid());
  EXPECT_THROW(SudokuSolver().solve(sudoku), std::exception);

  EXPECT_ANY_THROW(SudokuSolver().solve(Sudoku(
    "+--+--+\n"
    "|10|00|\n"
    "|01|00|\n"
    "+--+--+\n"
    "|00|00|\n"
    "|00|00|\n"
    "+--+--+\n"
  )));

  EXPECT_ANY_THROW(SudokuSolver().solve(Sudoku(
    "+--+--+\n"
    "|10|01|\n"
    "|00|00|\n"
    "+--+--+\n"
    "|00|00|\n"
    "|00|00|\n"
    "+--+--+\n"
  )));

  EXPECT_ANY_THROW(SudokuSolver().solve(Sudoku(
    "+--+--+\n"
    "|10|00|\n"
    "|00|00|\n"
    "+--+--+\n"
    "|00|00|\n"
    "|10|00|\n"
    "+--+--+\n"
  )));
}

TEST(SudokuSolver_test, almost_solved) {
  Sudoku sudoku(
    "+---+---+---+\n"
    "|681|739|245|\n"
    "|497|562|813|\n"
    "|523|841|769|\n"
    "+---+---+---+\n"
    "|172|954|386|\n"
    "|865|317|924|\n"
    "|349|628|571|\n"
    "+---+---+---+\n"
    "|916|283|457|\n"
    "|234|175|698|\n"
    "|758|496|132|\n"
    "+---+---+---+\n"
  );

  ASSERT_NO_THROW(SudokuSolver().solve(sudoku));

  for (unsigned i = 0; i < sudoku.size(); ++i) {
    for (unsigned digit = 1; digit <= 9; ++digit) {
      if (digit == sudoku[i]) {
	continue;
      }
      unsigned old = sudoku[i];
      sudoku[i] = digit;
      ASSERT_ANY_THROW(SudokuSolver().solve(sudoku));
      sudoku[i] = old;
    }
  }

  ASSERT_NO_THROW(SudokuSolver().solve(sudoku));
}

TEST(SudokuSolver_test, tenpai) {
  Sudoku sudoku(
    "+---+---+---+\n"
    "|681|739|245|\n"
    "|497|562|813|\n"
    "|523|841|769|\n"
    "+---+---+---+\n"
    "|172|954|386|\n"
    "|865|317|924|\n"
    "|349|628|571|\n"
    "+---+---+---+\n"
    "|916|283|457|\n"
    "|234|175|698|\n"
    "|758|496|132|\n"
    "+---+---+---+\n"
  );

  auto solved = sudoku;
  SudokuSolver solver;

  for (unsigned i = 0; i < sudoku.size(); ++i) {
    unsigned digit = sudoku[i];
    sudoku[i] = 0;
    ASSERT_EQ(solved, solver.solve(sudoku));
    sudoku[i] = digit;
  }
}

TEST(SudokuSolver_test, easy) {
  Sudoku easy(
    "+---+---+---+\n"
    "|14.|8..|97.|\n"
    "|..6|75.|...|\n"
    "|7..|...|..8|\n"
    "+---+---+---+\n"
    "|5..|4.2|.93|\n"
    "|93.|.7.|.82|\n"
    "|62.|9.8|..4|\n"
    "+---+---+---+\n"
    "|4..|...|..9|\n"
    "|...|.15|8..|\n"
    "|.72|..4|.15|\n"
    "+---+---+---+\n"
  );

  Sudoku solved(
    "+---+---+---+\n"
    "|145|823|976|\n"
    "|286|759|431|\n"
    "|793|146|528|\n"
    "+---+---+---+\n"
    "|518|462|793|\n"
    "|934|571|682|\n"
    "|627|938|154|\n"
    "+---+---+---+\n"
    "|451|387|269|\n"
    "|369|215|847|\n"
    "|872|694|315|\n"
    "+---+---+---+\n"
  );

  ASSERT_NE(solved, easy);
  EXPECT_EQ(solved, SudokuSolver().solve(easy));
}

TEST(SudokuSolver_test, hard) {
  Sudoku hard(
    "050|002|000"
    "000|100|400"
    "700|000|000"
    "---|---|---"
    "010|700|080"
    "004|030|060"
    "000|500|000"
    "---|---|---"
    "308|060|000"
    "000|000|100"
    "600|000|000"
  );

  Sudoku solved(
    "159|482|673"
    "836|175|429"
    "742|693|518"
    "---|---|---"
    "213|746|985"
    "584|239|761"
    "967|518|342"
    "---|---|---"
    "378|961|254"
    "495|827|136"
    "621|354|897"
  );

  EXPECT_EQ(solved, SudokuSolver().solve(hard));
}

TEST(SudokuSolver_test, region_2x2) {
  Sudoku puzzle(
    "21|.."
    ".3|2."
    "--+--"
    "..|.4"
    "1.|.."
  );
  Sudoku solved(
    "21|43"
    "43|21"
    "--+--"
    "32|14"
    "14|32"
  );
  EXPECT_EQ(solved, SudokuSolver().solve(puzzle));
}

TEST(SudokuSolver_test, region_3x2) {
  Sudoku puzzle(
    std::make_shared<SudokuType>(3, 2),
    "5.6|..."
    "...|.2."
    "---+---"
    ".6.|..."
    "...|5.1"
    "---+---"
    "..4|..."
    "...|1.3"
  );
  Sudoku solved(
    std::make_shared<SudokuType>(3, 2),
    "526|314"
    "431|625"
    "---+---"
    "165|432"
    "243|561"
    "---+---"
    "314|256"
    "652|143"
  );

  EXPECT_EQ(solved, SudokuSolver().solve(puzzle));
}

TEST(SudokuSolver_test, region_5x2) {
  Sudoku puzzle(
    std::make_shared<SudokuType>(5, 2),
    ".47..|..65."
    "9...5|7...2"
    "-----+-----"
    "6..41|85..9"
    "..92.|.63.."
    "-----+-----"
    ".92.8|6.47."
    ".A6.4|5.93."
    "-----+-----"
    "..36.|.91.."
    "8..79|3A..6"
    "-----+-----"
    "A...6|4...1"
    ".14..|..86."
  );
  Sudoku solved(
    std::make_shared<SudokuType>(5, 2),
    "147A2|98653"
    "96835|74A12"
    "-----+-----"
    "63A41|85729"
    "58927|163A4"
    "-----+-----"
    "39258|6147A"
    "7A614|52938"
    "-----+-----"
    "4536A|29187"
    "82179|3A546"
    "-----+-----"
    "A7586|43291"
    "21493|A7865"
  );

  EXPECT_EQ(solved, SudokuSolver().solve(puzzle));
}

TEST(SudokuSolver_test, custom_labels) {
  auto type = std::make_shared<SudokuType>(9);
  auto format = SudokuFormat(type).with_labels("ABDEIKPRW");

  Sudoku puzzle(type, format.get_values(
    ".P.|K.R|I.D"
    "D..|B..|..R"
    ".B.|E..|PA."
    "---+---+---"
    "P..|.KW|A.B"
    "...|...|RK."
    ".AD|...|..."
    "---+---+---"
    "B..|.E.|..P"
    "A..|...|E.."
    "ER.|P.K|B.."
  ));
  Sudoku solved(type, format.get_values(
    "WPE|KAR|IBD"
    "DIA|BWP|KER"
    "RBK|EID|PAW"
    "---+---+---"
    "PER|IKW|ADB"
    "IWB|DPA|RKE"
    "KAD|RBE|WPI"
    "---+---+---"
    "BKW|AEI|DRP"
    "ADP|WRB|EIK"
    "ERI|PDK|BWA"
  ));

  EXPECT_EQ(solved, SudokuSolver().solve(puzzle));
}

TEST(SudokuSolver_test, custom_regions) {
  auto type = std::make_shared<SudokuType>(std::vector<unsigned>{
    0, 0, 1, 1, 1, 1, 2,
    0, 0, 0, 1, 1, 1, 2,
    3, 0, 0, 4, 4, 2, 2,
    3, 3, 4, 4, 4, 2, 2,
    3, 3, 4, 4, 5, 5, 2,
    3, 6, 6, 6, 5, 5, 5,
    3, 6, 6, 6, 6, 5, 5,
  });

  Sudoku puzzle1(
    type,
    "---------------"
    "|. .|5 . . 6|4|"
    "|   ---     | |"
    "|2 . 1|. . .|7|"
    "|--   |------ |"
    "|.|. .|6 .|. .|"
    "| -----   |   |"
    "|. .|. . 5|. .|"
    "|   |   ----- |"
    "|4 3|. .|. .|.|"
    "| -------   --|"
    "|.|. . 5|4 . .|"
    "| |     ---   |"
    "|.|4 . 7 .|2 .|"
    "---------------"
  );

  Sudoku puzzle2(
    type,
    "---------------"
    "|. 1|3 . . 2|.|"
    "|   ---     | |"
    "|6 2 .|. . 5|.|"
    "|--   |------ |"
    "|.|. .|7 .|6 .|"
    "| -----   |   |"
    "|. .|. 4 .|. .|"
    "|   |   ----- |"
    "|1 .|. .|. .|.|"
    "| -------   --|"
    "|.|. . .|. . 6|"
    "| |     ---   |"
    "|2|. . . 6|. 7|"
    "---------------"
  );

  Sudoku solved2(
    type,
    "---------------"
    "|7 1|3 6 4 2|5|"
    "|   ---     | |"
    "|6 2 4|1 7 5|3|"
    "|--   |------ |"
    "|4|3 5|7 1|6 2|"
    "| -----   |   |"
    "|5 6|2 4 3|7 1|"
    "|   |   ----- |"
    "|1 7|6 5|2 3|4|"
    "| -------   --|"
    "|3|4 7 2|5 1 6|"
    "| |     ---   |"
    "|2|5 1 3 6|4 7|"
    "---------------"
  );

  EXPECT_ANY_THROW(SudokuSolver().solve(puzzle1));
  EXPECT_EQ(solved2, SudokuSolver().solve(puzzle2));
}

TEST(SudokuSolver_test, no_solution) {
  EXPECT_ANY_THROW(SudokuSolver().solve(Sudoku(
    "12|3."
    "..|.."
    "-----"
    "23|1."
    "..|.."
  )));
}

TEST(SudokuSolver_test, multiple_solutions) {
  EXPECT_NO_THROW(SudokuSolver().solve(Sudoku(
    "12|34"
    ".3|21"
    "-----"
    "..|12"
    "21|43"
  )));

  EXPECT_ANY_THROW(SudokuSolver().solve(Sudoku(
    "12|34"
    "..|21"
    "-----"
    "..|12"
    "21|43"
  )));
}

}
