#include "Polyomino.hpp"

Polyomino::Polyomino()
  : width_(10),
  height_(6)
{
  shapes_ = {
    Shape('I', {
      {1, 1, 1, 1, 1}
    }).variations(),

    Shape('N', {
      {1, 1, 1, 0},
      {0, 0, 1, 1},
    }).variations(),
    Shape('L', {
      {1, 1, 1, 1},
      {1, 0, 0, 0},
    }).variations(),
    Shape('Y', {
      {1, 1, 1, 1},
      {0, 1, 0, 0},
    }).variations(),

    Shape('P', {
      {1, 1, 1},
      {1, 1, 0},
    }).variations(),
    Shape('C', {
      {1, 1, 1},
      {1, 0, 1},
    }).variations(),

    Shape('V', {
      {1, 1, 1},
      {1, 0, 0},
      {1, 0, 0},
    }).variations(),
    Shape('T', {
      {1, 1, 1},
      {0, 1, 0},
      {0, 1, 0},
    }).variations(),
    Shape('F', {
      {1, 1, 0},
      {0, 1, 1},
      {0, 1, 0},
    }).variations(),
    Shape('Z', {
      {1, 1, 0},
      {0, 1, 0},
      {0, 1, 1},
    }).variations(),
    Shape('W', {
      {1, 1, 0},
      {0, 1, 1},
      {0, 0, 1},
    }).variations(),
    Shape('X', {
      {0, 1, 0},
      {1, 1, 1},
      {0, 1, 0},
    }).variations(),
  };

  problem_ = ExactCoverProblem(shapes_.size() + width_ * height_);
  auto size = width_ * height_;
  for (auto s = 0u; s < shapes_.size(); ++s) {
    for (auto v = 0u; v < shapes_[s].size(); ++v) {
      const auto& shape = shapes_[s][v];
      for (auto yx = 0u; yx < size; ++yx) {
        auto y = yx / width_;
        auto x = yx % width_;
        if (x + shape.width() > width_ || y + shape.height() > height_) {
          continue;
        }
        if (shape.name() == 'X' && (width_ - x - shape.width() < x || height_ - y - shape.height() < y)) {
          continue;
        }
        row_data_.push_back({s, v, x, y});
        auto row = std::vector<unsigned>();
        for (auto dyx = 0u; dyx < shape.size(); ++dyx) {
          if (!shape[dyx]) {
            continue;
          }
          auto dy = dyx / shape.width();
          auto dx = dyx % shape.width();
          row.push_back((y + dy) * width_ + x + dx);
        }
        row.push_back(size + s);
        problem_.add_row(std::move(row));
      }
    }
  }
}

auto Polyomino::problem() const -> const ExactCoverProblem& {
  return problem_;
}

auto Polyomino::make_solution(const std::vector<unsigned>& used_rows) const -> std::vector<std::string> {
  auto lines = std::vector<std::string>(height_, std::string(width_, '.'));
  for (auto i : used_rows) {
    const auto& data = row_data_[i];
    const auto& shape = shapes_[data.shape][data.variation];
    for (auto y = 0u; y < shape.height(); ++y) {
      for (auto x = 0u; x < shape.width(); ++x) {
        if (shape[y * shape.width() + x]) {
          lines[data.y + y][data.x + x] = shape.name();
        }
      }
    }
  }
  return lines;
}
