#include "SudokuType.hpp"
#include "SudokuFormat.hpp"

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
  region_(normalize_regions(std::move(regions)))
{
  if (n_ == 0) {
    throw std::invalid_argument("Sudoku must have non-zero size");
  }
}

std::shared_ptr<SudokuType> SudokuType::from_size(unsigned size) {
  return std::make_shared<SudokuType>(isqrt(size));
}

std::shared_ptr<SudokuType> SudokuType::guess(const std::string& str) {
  return from_size(SudokuFormat::count_cells(str));
}

bool SudokuType::operator==(const SudokuType& other) const {
  return region_ == other.region_;
}

bool SudokuType::operator!=(const SudokuType& other) const {
  return !(region_ == other.region_);
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

unsigned SudokuType::isqrt(unsigned n) {
  unsigned k = 0;
  while ((k + 1) * (k + 1) <= n) ++k;
  if (k * k != n) {
    throw std::invalid_argument("Not a square");
  }
  return k;
}
