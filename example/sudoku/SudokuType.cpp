#include "SudokuType.hpp"

#include <assert.h>
#include <unordered_map>
#include <unordered_set>

SudokuType::SudokuType()
  : SudokuType(9)
{
}

SudokuType::SudokuType(unsigned size)
  : SudokuType(isqrt(size), isqrt(size))
{
}

SudokuType::SudokuType(unsigned region_width, unsigned region_height)
  : SudokuType(box_regions(region_width, region_height))
{
}

SudokuType::SudokuType(std::initializer_list<unsigned> regions)
  : SudokuType(std::vector<unsigned>(regions))
{
}

SudokuType::SudokuType(std::vector<unsigned> regions)
  : n_(isqrt(regions.size())),
  empty_chars_{'.', '0'},
  labels_(default_labels(n_)),
  region_(normalize_regions(std::move(regions)))
{
  if (n_ == 0) {
    throw std::invalid_argument("Sudoku must have non-zero size");
  }
}

std::shared_ptr<SudokuType> SudokuType::fromSize(unsigned size) {
  return std::make_shared<SudokuType>(isqrt(size));
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

unsigned SudokuType::region(unsigned pos) const {
  assert(pos < size());
  return region_[pos];
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

std::vector<unsigned> SudokuType::normalize_regions(std::vector<unsigned> regions) {
  unsigned n = isqrt(regions.size());
  std::unordered_map<unsigned, unsigned> ids;
  std::unordered_map<unsigned, unsigned> areas;
  for (unsigned id : regions) {
    if (ids.find(id) == ids.end()) {
      unsigned size = ids.size();
      ids[id] = size;
    }
    if (++areas[id] > n) {
      throw std::invalid_argument("Region has wrong size");
    }
  }
  if (ids.size() != n) {
    throw std::invalid_argument("Too many regions");
  }
  for (unsigned& id : regions) {
    id = ids[id];
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
  if (k * k != n) {
    throw std::invalid_argument("Not a square");
  }
  return k;
}
