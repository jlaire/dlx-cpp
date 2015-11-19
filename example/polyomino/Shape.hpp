#pragma once

#include <vector>

struct Shape
{
  Shape();
  Shape(std::initializer_list<std::vector<unsigned>> bits);
  Shape(char name, std::vector<std::vector<unsigned>> bits);

  auto rotate() const -> Shape;
  auto reflect() const -> Shape;
  auto rotations() const -> std::vector<Shape>;
  auto reflections() const -> std::vector<Shape>;
  auto variations() const -> std::vector<Shape>;

  char name;
  std::vector<std::vector<unsigned>> bits;
  unsigned width;
  unsigned height;
};

bool operator==(const Shape&, const Shape&);
bool operator!=(const Shape&, const Shape&);
bool operator<(const Shape&, const Shape&);
