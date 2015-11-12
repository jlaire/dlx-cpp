#include "SudokuType.hpp"

#include <assert.h>

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
  alphabet_(default_alphabet(isqrt(regions.size()))),
  region_(std::move(regions))
{
}

bool SudokuType::is_cell(char c) const {
  return empty_chars_.find(c) != std::string::npos
    || alphabet_.find(c) != std::string::npos;
}

unsigned SudokuType::value(char c) const {
  auto pos = alphabet_.find(c);
  return pos == std::string::npos ? 0 : pos + 1;
}

char SudokuType::label(unsigned i) const {
  assert(i < n_);
  return alphabet_[i];
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

void SudokuType::set_alphabet(std::string alphabet) {
  assert(alphabet.size() == n_);
  alphabet_ = std::move(alphabet);
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

std::string SudokuType::default_alphabet(unsigned n) {
  static const std::string chars(
    "123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
  );

  if (n > chars.size()) {
    throw SudokuTooBigException();
  }

  std::string alphabet;
  for (unsigned i = 0; i < n; ++i) {
    alphabet.push_back(chars[i]);
  }
  return alphabet;
}

unsigned SudokuType::isqrt(unsigned n) {
  unsigned k = 0;
  while ((k + 1) * (k + 1) <= n) ++k;
  return k;
}
