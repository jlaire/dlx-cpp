#include "SudokuFormat.hpp"

#include <assert.h>
#include <unordered_set>

SudokuFormat::SudokuFormat(std::string format)
  : SudokuFormat{SudokuType::guess(format), std::move(format)}
{
}

SudokuFormat::SudokuFormat(std::shared_ptr<SudokuType> type)
  : SudokuFormat(type, default_template(*type))
{
}

SudokuFormat::SudokuFormat(std::shared_ptr<SudokuType> type, std::string format)
  : type_(type),
  template_(std::move(format)),
  labels_(default_labels(type_->n()))
{
  unsigned cells = 0;
  for (char& c : template_) {
    if (is_cell(c)) {
      c = '.';
      ++cells;
    }
  }
  if (cells != type_->size()) {
    throw std::invalid_argument("");
  }
}

SudokuFormat SudokuFormat::compact(std::shared_ptr<SudokuType> type) {
  std::string line(type->n(), '.');
  line += '\n';
  std::string result;
  for (unsigned y = 0; y < type->n(); ++y) {
    result += line;
  }
  return SudokuFormat(type, result);
}

SudokuFormat SudokuFormat::oneline(std::shared_ptr<SudokuType> type) {
  return SudokuFormat(type, std::string(type->size(), '.') + '\n');
}

SudokuFormat SudokuFormat::with_labels(std::string labels) const {
  if (labels.size() != type_->n()) {
    throw std::invalid_argument("Wrong number of labels");
  }

  std::unordered_set<char> used;
  for (char c : labels) {
    if (!is_valid_label(c)) {
      throw std::invalid_argument("Invalid label");
    }
    if (used.find(c) != used.end()) {
      throw std::invalid_argument("Cannot use same label twice");
    }
    used.insert(c);
  }

  auto format = *this;
  format.labels_ = std::move(labels);
  return format;
}

unsigned SudokuFormat::count_cells(const std::string& str) {
  unsigned count = 0;
  for (char c : str) {
    if (is_valid_label(c) || is_empty(c)) {
      ++count;
    }
  }
  return count;
}

bool SudokuFormat::is_cell(char c) const {
  return is_empty(c) || labels_.find(c) != std::string::npos;
}

unsigned SudokuFormat::value(char c) const {
  auto pos = labels_.find(c);
  return pos == std::string::npos ? 0 : pos + 1;
}

char SudokuFormat::label(unsigned i) const {
  assert(i <= labels_.size());
  return i == 0 ? '.' : labels_[i - 1];
}

std::vector<unsigned> SudokuFormat::get_values(const std::string& str) const {
  std::vector<unsigned> values;
  for (char c : str) {
    if (is_cell(c)) {
      values.push_back(value(c));
    }
  }
  if (values.size() != type_->size()) {
    throw std::invalid_argument("get_values(): wrong number of elements");
  }
  return values;
}

std::string SudokuFormat::to_string() const {
  return to_string(std::vector<unsigned>(type_->size(), 0));
}

std::string SudokuFormat::to_string(const std::vector<unsigned>& values) const {
  if (values.size() != type_->size()) {
    throw std::invalid_argument("to_string(): wrong number of values");
  }

  std::string result = template_;
  unsigned j = 0;
  for (unsigned i = 0; i < result.size(); ++i) {
    if (is_cell(result[i])) {
      if (j >= values.size()) {
	throw std::logic_error("");
      }
      result[i] = label(values[j]);
      ++j;
    }
  }
  if (j != values.size()) {
    throw std::logic_error("");
  }
  return result;
}

const SudokuType& SudokuFormat::type() const {
  return *type_;
}

bool SudokuFormat::is_empty(char c) {
  return c == '.' || c == '0';
}

bool SudokuFormat::is_valid_label(char c) {
  return valid_labels().find(c) != std::string::npos;
}

const std::string& SudokuFormat::valid_labels() {
  static const std::string chars(
    "123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
  );
  return chars;
}

std::string SudokuFormat::default_labels(unsigned n) {
  if (n > valid_labels().size()) {
    throw std::invalid_argument("Sudoku too large");
  }

  return valid_labels().substr(0, n);
}

std::string SudokuFormat::default_template(const SudokuType& type) {
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