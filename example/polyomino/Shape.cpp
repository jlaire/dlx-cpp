#include "Shape.hpp"

#include <assert.h>
#include <algorithm>

Shape::Shape()
  : Shape(std::initializer_list<std::vector<unsigned>>{})
{
}

Shape::Shape(std::initializer_list<std::vector<unsigned>> bits)
  : Shape('#', bits)
{
}

Shape::Shape(char name, std::vector<std::vector<unsigned>> bs)
  : name(name),
  bits(std::move(bs)),
  width(bits.empty() ? 0 : bits[0].size()),
  height(bits.size())
{
  for (const auto& row : bits) {
    assert(row.size() == width);
  }
}

Shape Shape::rotate() const {
  auto rows = std::vector<std::vector<unsigned>>(width, std::vector<unsigned>(height));
  for (auto y = 0u; y < height; ++y) {
    for (auto x = 0u; x < width; ++x) {
      rows[x][height - y - 1] = bits[y][x];
    }
  }
  return Shape(name, rows);
}

Shape Shape::reflect() const {
  auto rows = bits;
  for (auto& row : rows) {
    std::reverse(row.begin(), row.end());
  }
  return Shape(name, rows);
}

std::vector<Shape> Shape::rotations() const {
  auto result = std::vector<Shape>{*this};
  for (auto shape = rotate(); shape != result[0]; shape = shape.rotate()) {
    result.push_back(shape);
  }
  return result;
}

std::vector<Shape> Shape::reflections() const {
  auto refl = reflect();
  for (const auto& rot : rotations()) {
    if (rot == refl) {
      return {*this};
    }
  }
  return {*this, refl};
}

std::vector<Shape> Shape::variations() const {
  auto vars = std::vector<Shape>();
  for (const auto& refl : reflections()) {
    for (auto&& rot : refl.rotations()) {
      vars.push_back(std::move(rot));
    }
  }
  return vars;
}

bool operator==(const Shape& a, const Shape& b) {
  return a.bits == b.bits;
}

bool operator!=(const Shape& a, const Shape& b) {
  return !(a == b);
}

bool operator<(const Shape& a, const Shape& b) {
  return a.bits < b.bits;
}
