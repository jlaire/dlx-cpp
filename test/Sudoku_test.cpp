#include "../example/sudoku/SudokuSolver.hpp"
#include "../example/sudoku/SudokuSolver.cpp"
#include "../example/sudoku/SudokuType.hpp"
#include "../example/sudoku/SudokuType.cpp"

#include <gtest/gtest.h>

namespace {

TEST(Sudoku_test, already_solved) {
  std::string solved("846937152319625847752184963285713694463859271971246385127598436638471529594362718");
  EXPECT_EQ(solved, SudokuSolver().solve(solved));
}

TEST(Sudoku_test, non_digits) {
  std::string solved(
    "+---+---+---+\n"
    "|145|327|698|\n"
    "|839|654|127|\n"
    "|672|918|543|\n"
    "+---+---+---+\n"
    "|496|185|372|\n"
    "|218|473|956|\n"
    "|753|296|481|\n"
    "+---+---+---+\n"
    "|367|542|819|\n"
    "|984|761|235|\n"
    "|521|839|764|\n"
    "+---+---+---+\n"
  );
  EXPECT_EQ(solved, SudokuSolver().solve(solved));
}

TEST(Sudoku_test, invalid) {
  EXPECT_EQ("Invalid sudoku! Not enough digits.", SudokuSolver().solve(std::string(80, '1')));
  EXPECT_EQ("Invalid sudoku! Too many digits.", SudokuSolver().solve(std::string(82, '1')));

  auto str = SudokuSolver().solve(std::string(81, '1'));
  EXPECT_EQ(std::string::npos, str.find("Invalid sudoku!"));
}

TEST(Sudoku_test, contradictory) {
  EXPECT_EQ("Unsolvable sudoku :(", SudokuSolver().solve(
    "+---+---+---+\n"
    "|100|000|000|\n"
    "|010|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
  ));

  EXPECT_EQ("Unsolvable sudoku :(", SudokuSolver().solve(
    "+---+---+---+\n"
    "|100|010|000|\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
  ));

  EXPECT_EQ("Unsolvable sudoku :(", SudokuSolver().solve(
    "+---+---+---+\n"
    "|100|000|000|\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
    "|000|000|000|\n"
    "|100|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "|000|000|000|\n"
    "+---+---+---+\n"
  ));
}

TEST(Sudoku_test, almost_solved) {
  std::string solved(
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

  auto copy = solved;
  unsigned count = 0;
  for (char& c : copy) {
    if (::isdigit(c)) {
      for (char digit = '1'; digit <= '9'; ++digit) {
	if (digit == c) {
	  continue;
	}
	++count;
	char old = c;
	c = digit;
	ASSERT_EQ("Unsolvable sudoku :(", SudokuSolver().solve(copy));
	c = old;
      }
    }
  }

  ASSERT_EQ(8 * 9 * 9, count);
}

TEST(Sudoku_test, tenpai) {
  std::string solved(
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

  SudokuSolver solver;

  auto copy = solved;
  unsigned count = 0;
  for (char& c : copy) {
    if (::isdigit(c)) {
      ++count;
      char old = c;
      c = '0';
      ASSERT_EQ(solved, solver.solve(copy));
      c = '.';
      ASSERT_EQ(solved, solver.solve(copy));
      c = old;
    }
  }

  ASSERT_EQ(9 * 9, count);
}

TEST(Sudoku_test, easy) {
  std::string easy(
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

  std::string solved(
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

  EXPECT_EQ(solved, SudokuSolver().solve(easy));
}

TEST(Sudoku_test, hard) {
  std::string hard(
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

  std::string solved(
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

TEST(Sudoku_test, region_2x2) {
  std::string puzzle(
    "21|.."
    ".3|2."
    "--+--"
    "..|.4"
    "1.|.."
  );
  std::string solved(
    "21|43"
    "43|21"
    "--+--"
    "32|14"
    "14|32"
  );
  EXPECT_EQ(solved, SudokuSolver(2).solve(puzzle));
}

TEST(Sudoku_test, region_3x2) {
  std::string puzzle(
    "5.6|..."
    "...|.2."
    "---+---"
    ".6.|..."
    "...|5.1"
    "---+---"
    "..4|..."
    "...|1.3"
  );
  std::string solved(
    "526|314"
    "431|625"
    "---+---"
    "165|432"
    "243|561"
    "---+---"
    "314|256"
    "652|143"
  );
  EXPECT_EQ(solved, SudokuSolver(3, 2).solve(puzzle));
}

TEST(Sudoku_test, region_5x2) {
  std::string puzzle(
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
  std::string solved(
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

  EXPECT_EQ(solved, SudokuSolver(5, 2).solve(puzzle));
}

TEST(Sudoku_test, custom_alphabet) {
  std::string puzzle(
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
  );
  std::string solved(
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
  );

  SudokuType type;
  type.set_alphabet("ABDEIKPRW");
  SudokuSolver solver(type);
  EXPECT_EQ(solved, solver.solve(puzzle));
}

TEST(Sudoku_test, custom_regions) {
  SudokuSolver sudoku(std::vector<unsigned>{
    0, 0, 1, 1, 1, 1, 2,
    0, 0, 0, 1, 1, 1, 2,
    3, 0, 0, 4, 4, 2, 2,
    3, 3, 4, 4, 4, 2, 2,
    3, 3, 4, 4, 5, 5, 2,
    3, 6, 6, 6, 5, 5, 5,
    3, 6, 6, 6, 6, 5, 5,
  });

  std::string puzzle1(
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

  std::string puzzle2(
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

  std::string solved2(
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

  EXPECT_EQ("No solution!", sudoku.solve(puzzle1));
  EXPECT_EQ(solved2, sudoku.solve(puzzle2));
}

}
