#include "Sudoku.hpp"

#include <algorithm>

Sudoku::Sudoku()
  : Sudoku(SudokuType())
{
}

Sudoku::Sudoku(SudokuType type)
  : Sudoku(type, default_template(type))
{
}

Sudoku::Sudoku(const std::string& str)
  : Sudoku(guess_type(str), str)
{
}

Sudoku::Sudoku(SudokuType type, const std::string& str)
  : type_(std::move(type)),
  template_(parse_template(str)),
  values_(get_values(type_, str))
{
}

std::string Sudoku::default_template(const SudokuType& type) {
  /*
   * The template is built in four steps.
   *
   * 1) empty
   *
   * +-------+
   * |. . . .|
   * |       |
   * |. . . .|
   * |       |
   * |. . . .|
   * |       |
   * |. . . .|
   * +-------+
   *
   * 2) add vertical lines
   *
   * +-----+-+
   * |. . .|.|
   * | |   | |
   * |.|. . .|
   * | | |   |
   * |. .|. .|
   * |   |   |
   * |. .|. .|
   * +---+---+
   *
   * 3) add horizontal lines
   *
   * +-----+-+
   * |. . .|.|
   * | +---+ |
   * |.|. . .|
   * +-+-+---+
   * |. .|. .|
   * |   |   |
   * |. .|. .|
   * +---+---+
   *
   * 4) collapse uninteresting rows and columns
   *
   * +-----+-+
   * |. . .|.|
   * | +---+ |
   * |.|. . .|
   * +-+-+---+
   * |. .|. .|
   * |. .|. .|
   * +---+---+
   *
   */

  // step 1: empty
  std::vector<std::string> lines;
  unsigned n = type.n();
  std::string header(n * 2 + 1, '-');
  std::string empty(n * 2 + 1, ' ');
  *header.begin() = '+';
  *header.rbegin() = '+';
  *empty.begin() = '|';
  *empty.rbegin() = '|';

  lines.push_back(header);
  for (unsigned i = 0; i < 2 * n - 1; ++i) {
    lines.push_back(empty);
  }
  lines.push_back(header);

  for (unsigned y = 0; y < n; ++y) {
    for (unsigned x = 0; x < n; ++x) {
      lines[2 * y + 1][2 * x + 1] = '.';
    }
  }

  // step 2: add vertical lines
  auto set = [&](unsigned x, unsigned y, char c) {
    if (lines[y][x] == ' ') {
      lines[y][x] = c;
    }
    else if (lines[y][x] != c) {
      lines[y][x] = '+';
    }
  };

  for (unsigned y = 0; y < n; ++y) {
    for (unsigned x = 0; x + 1 < n; ++x) {
      if (type.region(x, y) != type.region(x + 1, y)) {
	set(2 * x + 2, 2 * y, '|');
	set(2 * x + 2, 2 * y + 1, '|');
	set(2 * x + 2, 2 * y + 2, '|');
      }
    }
  }

  // step 3: add horizontal lines
  for (unsigned y = 0; y + 1 < n; ++y) {
    for (unsigned x = 0; x < n; ++x) {
      if (type.region(x, y) != type.region(x, y + 1)) {
	set(2 * x, 2 * y + 2, '-');
	set(2 * x + 1, 2 * y + 2, '-');
	set(2 * x + 2, 2 * y + 2, '-');
      }
    }
  }

  // step 4: collapse uninteresting rows and columns
  unsigned Y = lines.size();
  unsigned X = lines[0].size();
  std::vector<bool> keep_row(Y);
  std::vector<bool> keep_col(X);
  for (unsigned y = 0; y < Y; ++y) {
    for (unsigned x = 0; x < X; ++x) {
      char c = lines[y][x];
      if (c != ' ' && c != '|') {
	keep_row[y] = true;
      }
      if (c != ' ' && c != '-') {
	keep_col[x] = true;
      }
    }
  }

  std::string result;
  for (unsigned y = 0; y < Y; ++y) {
    if (!keep_row[y]) {
      continue;
    }
    for (unsigned x = 0; x < X; ++x) {
      if (keep_col[x]) {
	result += lines[y][x];
      }
    }
    result += '\n';
  }
  return result;
}

std::string Sudoku::default_template() const {
  return default_template(type_);
}

void Sudoku::set_template(const std::string& str) {
  template_ = parse_template(str);
}

bool Sudoku::is_valid() const {
  unsigned n = type_.n();
  for (unsigned i = 0; i < type_.size(); ++i) {
    for (unsigned j = i + 1; j < type_.size(); ++j) {
      unsigned a = values_[i];
      unsigned b = values_[j];
      if (a == 0 || a != b) {
	continue;
      }
      if (i % n == j % n) {
	return false;
      }
      if (i / n == j / n) {
	return false;
      }
      if (type_.region(i) == type_.region(j)) {
	return false;
      }
    }
  }
  return true;
}

bool Sudoku::is_solved() const {
  return is_valid() && *std::min_element(values_.begin(), values_.end()) > 0;
}

unsigned& Sudoku::operator[](unsigned pos) {
  return values_[pos];
}

unsigned Sudoku::operator[](unsigned pos) const {
  return values_[pos];
}

std::string Sudoku::to_string() const {
  return template_;
}

SudokuType Sudoku::guess_type(const std::string& str) {
  return SudokuType();
}

std::string Sudoku::parse_template(const std::string& str) {
  return str;
}

std::vector<unsigned> Sudoku::get_values(const SudokuType& type, const std::string& str) {
  return std::vector<unsigned>(type.size());
}
