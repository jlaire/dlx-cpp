#include "NPieces.hpp"

#include <dlx/GeneralizedAlgorithmDLX.hpp>
#include <algorithm>
#include <set>

NPieces NPieces::size(unsigned n) const {
  return size(n, n);
}

NPieces NPieces::size(unsigned width, unsigned height) const {
  return NPieces(width, height, knights_, queens_);
}

NPieces NPieces::knights(unsigned n) const {
  return NPieces(width_, height_, n, queens_);
}

NPieces NPieces::queens(unsigned n) const {
  return NPieces(width_, height_, knights_, n);
}

NPieces::NPieces(unsigned width, unsigned height, unsigned knights, unsigned queens)
  : width_(width),
  height_(height),
  knights_(knights),
  queens_(queens)
{
  // Columns
  //   A: (x,y) attacked (epsilon) or occupied (1)?
  //   PA: piece p ok at (x,y)? (to enfore ordering)
  //   P: piece i used?
  //
  // Total: A + PA + P
  // Secondary: A + PA

  auto A = width_ * height_;
  auto P = knights_ + queens_;

  auto piece_type = [&](unsigned p) -> Piece {
    return p < knights_ ? Piece::Knight : Piece::Queen;
  };
  auto col_attack = [&](unsigned yx) -> int {
    return 1 + yx;
  };
  auto col_ok = [&](unsigned p, unsigned yx) -> int {
    return 1 + (p + 1) * A + yx;
  };
  auto col_piece = [&](unsigned p) -> int {
    return 1 + (P + 1) * A + p;
  };

  for (auto yx = 0u; yx < A; ++yx) {
    auto x = yx % width_;
    auto y = yx / width_;
    for (auto p = 0u; p < P; ++p) {
      auto piece = piece_type(p);
      row_data_.push_back({piece, x, y});
      auto columns = std::vector<int>{
        col_attack(yx),
        col_ok(p, yx),
        col_piece(p),
      };
      for (auto pos : attacks(piece, x, y)) {
        columns.push_back(-col_attack(pos));
      }
      if (p > 0 && piece_type(p - 1) == piece) {
        for (auto yx2 = yx + 1; yx2 < A; ++yx2) {
          columns.push_back(col_ok(p - 1, yx2));
        }
      }
      std::sort(columns.begin(), columns.end(), [](int a, int b) { return abs(a) < abs(b); });
      rows_.push_back(std::move(columns));
    }
  }
}

unsigned NPieces::count_solutions() const {
  auto A = width_ * height_;
  auto P = knights_ + queens_;
  auto lm = GeneralizedLinkedMatrix::make(A + P * A + P + 1, rows_, A + P * A + 1);
  auto dlx = GeneralizedAlgorithmDLX(std::move(lm));
  return dlx.count_solutions();
}

auto NPieces::find_solutions() const -> std::vector<Solution> {
  auto A = width_ * height_;
  auto P = knights_ + queens_;
  auto lm = GeneralizedLinkedMatrix::make(A + P * A + P + 1, rows_, A + P * A + 1);
  auto dlx = GeneralizedAlgorithmDLX(std::move(lm));
  auto solutions = std::vector<Solution>();
  for (const auto& used_rows : dlx.find_solutions()) {
    auto solution = Solution(height_, std::vector<Piece>(width_, Piece::None));
    for (auto i : used_rows) {
      auto data = row_data_[i];
      solution[data.y][data.x] = data.piece;
    }
    solutions.push_back(std::move(solution));
  }
  return solutions;
}

std::vector<unsigned> NPieces::attacks(Piece piece, unsigned x0, unsigned y0) const {
  auto points = std::set<unsigned>();
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      if (x == x0 && y == y0) {
        continue;
      }
      if (is_attack(piece, x0, y0, x, y)) {
        points.insert(y * width_ + x);
      }
    }
  }

  auto result = std::vector<unsigned>();
  result.assign(points.begin(), points.end());
  return result;
}

bool NPieces::is_attack(Piece piece, int x1, int y1, int x2, int y2) const {
  if (piece == Piece::Knight) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy == 5;
  }
  if (piece == Piece::Queen) {
    return x1 == x2 || y1 == y2 || y1 - x1 == y2 - x2 || y1 + x1 == y2 + x2;
  }
  throw std::logic_error("NPieces::is_attack(): Unknown piece");
}
