#include "SudokuType.hpp"

#include <assert.h>
#include <unordered_set>

SudokuType::SudokuType()
  : SudokuType(3)
{
}

SudokuType::SudokuType(unsigned region_size)
  : SudokuType(region_size, region_size)
{
}

SudokuType::SudokuType(unsigned region_width, unsigned region_height)
  : SudokuType(box_regions(region_width, region_height))
{
}

SudokuType::SudokuType(std::vector<unsigned> regions)
  : n_(isqrt(regions.size())),
  empty_chars_{'.', '0'},
  labels_(default_labels(isqrt(regions.size()))),
  region_(std::move(regions))
{
  if (n_ == 0) {
    throw std::invalid_argument("Sudoku must have non-zero size");
  }
}

bool SudokuType::is_cell(char c) const {
  return empty_chars_.find(c) != std::string::npos
    || labels_.find(c) != std::string::npos;
}

unsigned SudokuType::value(char c) const {
  auto pos = labels_.find(c);
  return pos == std::string::npos ? 0 : pos + 1;
}

char SudokuType::label(unsigned i) const {
  assert(i < n_);
  return labels_[i];
}

unsigned SudokuType::n() const {
  return n_;
}

unsigned SudokuType::size() const {
  return n_ * n_;
}

unsigned SudokuType::region(unsigned x, unsigned y) const {
  assert(x < n_ && y < n_);
  return region_[y * n_ + x];
}

void SudokuType::set_labels(std::string labels) {
  if (labels.size() != n_) {
    throw std::invalid_argument("Wrong number of labels");
  }

  std::unordered_set<char> used;
  for (char c : labels) {
    if (empty_chars_.find(c) != std::string::npos) {
      throw std::invalid_argument("Cannot use empty char as label");
    }
    if (used.find(c) != used.end()) {
      throw std::invalid_argument("Cannot use same label twice");
    }
    used.insert(c);
  }

  labels_ = std::move(labels);
}

std::vector<unsigned> SudokuType::box_regions(unsigned w, unsigned h) {
  std::vector<unsigned> regions;
  unsigned n = w * h;
  for (unsigned y = 0; y < n; ++y) {
    for (unsigned x = 0; x < n; ++x) {
      regions.push_back(y / h * h + x / w);
    }
  }
  return regions;
}

std::string SudokuType::default_labels(unsigned n) {
  static const std::string chars(
    "123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
  );

  if (n > chars.size()) {
    throw std::invalid_argument("Sudoku too large");
  }

  std::string labels;
  for (unsigned i = 0; i < n; ++i) {
    labels.push_back(chars[i]);
  }
  return labels;
}

unsigned SudokuType::isqrt(unsigned n) {
  unsigned k = 0;
  while ((k + 1) * (k + 1) <= n) ++k;
  return k;
}
