#include "../example/sudoku/Sudoku.hpp"
#include "../example/sudoku/Sudoku.cpp"

#include <gtest/gtest.h>

namespace {

TEST(Sudoku_test, already_solved) {
  std::string solved("846937152319625847752184963285713694463859271971246385127598436638471529594362718");
  EXPECT_EQ(solved, Sudoku::solve(solved));
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
  EXPECT_EQ(solved, Sudoku::solve(solved));
}

TEST(Sudoku_test, invalid) {
  EXPECT_EQ("Invalid sudoku! Not enough digits.", Sudoku::solve(std::string(80, '1')));
  EXPECT_EQ("Invalid sudoku! Too many digits.", Sudoku::solve(std::string(82, '1')));

  auto str = Sudoku::solve(std::string(81, '1'));
  EXPECT_EQ(std::string::npos, str.find("Invalid sudoku!"));
}

TEST(Sudoku_test, contradictory) {
  EXPECT_EQ("Unsolvable sudoku :(", Sudoku::solve(
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

  EXPECT_EQ("Unsolvable sudoku :(", Sudoku::solve(
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

  EXPECT_EQ("Unsolvable sudoku :(", Sudoku::solve(
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
	ASSERT_EQ("Unsolvable sudoku :(", Sudoku::solve(copy));
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

  auto copy = solved;
  unsigned count = 0;
  for (char& c : copy) {
    if (::isdigit(c)) {
      ++count;
      char old = c;
      c = '0';
      ASSERT_EQ(solved, Sudoku::solve(copy));
      c = '.';
      ASSERT_EQ(solved, Sudoku::solve(copy));
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

  EXPECT_EQ(solved, Sudoku::solve(easy));
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

  EXPECT_EQ(solved, Sudoku::solve(hard));
}

}
