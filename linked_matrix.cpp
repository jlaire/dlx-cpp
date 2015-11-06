#include "linked_matrix.hpp"

#include <algorithm>

void Box::hide_lr(linked_matrix& M) { M[l].r = r, M[r].l = l; }
void Box::show_lr(linked_matrix& M) { M[l].r = M[r].l = id; }
void Box::hide_ud(linked_matrix& M) { M[u].d = d, M[d].u = u; }
void Box::show_ud(linked_matrix& M) { M[u].d = M[d].u = id; }
void Box::link_l(linked_matrix& M, Box& b) { b.r = id, b.l = l; l = M[l].r = b.id; }
void Box::link_u(linked_matrix& M, Box& b) { b.d = id, b.u = u; u = M[u].d = b.id; }

std::unique_ptr<linked_matrix> linked_matrix::from_boolean_rows(
    const VectorVector& rows, unsigned secondary)
{
  VectorVector sparse(rows.size());
  for (unsigned i = 0; i < rows.size(); ++i) {
    for (unsigned j = 0; j < rows[i].size(); ++j) {
      if (rows[i][j]) {
        sparse[i].push_back(j);
      }
    }
  }
  return from_sparse_matrix(sparse, secondary, rows[0].size());
}

std::unique_ptr<linked_matrix> linked_matrix::from_sparse_matrix(
    const VectorVector& rows, unsigned secondary, unsigned width)
{
  std::unique_ptr<linked_matrix> lm{new linked_matrix};
  Box& root = lm->create_box(~0, ~0);
  lm->root_id = root.id;
  if (rows.empty()) {
    return lm;
  }

  for (auto& row : rows) {
    for (unsigned x : row) {
      width = std::max(width, x + 1);
    }
  }

  lm->col_ids.resize(width);
  lm->sizes.resize(width);
  for (unsigned x = 0; x < width; ++x) {
    lm->sizes[x] = 0;
    Box& col = lm->create_box(x, ~0);
    lm->col_ids[x] = col.id;
    if (x >= secondary) {
      lm->root().link_l(*lm, col);
    }
  }
  for (unsigned y = 0; y < rows.size(); ++y) {
    auto& xs = rows[y];
    BoxId row_id;
    {
      Box& row = lm->create_box(~0, y);
      row_id = row.id;
    }
    for (unsigned x : xs) {
      if (x >= width) {
        return nullptr;
      }
      Box& cell = lm->create_box(x, y);
      lm->box(lm->col_ids[x]).link_u(*lm, cell);
      ++lm->sizes[x];
      lm->box(row_id).link_l(*lm, cell);
    }
    lm->box(row_id).hide_lr(*lm);
  }
  return lm;
}

void linked_matrix::cover_column(unsigned x) {
  BoxId col_id = col_ids[x];
  box(col_id).hide_lr(*this);
  for (BoxId a = box(col_id).d; a != col_id; a = box(a).d) {
    for (BoxId b = box(a).r; b != a; b = box(b).r) {
      box(b).hide_ud(*this);
      --sizes[box(b).x];
    }
  }
}

void linked_matrix::uncover_column(unsigned x) {
  BoxId col_id = col_ids[x];
  for (BoxId a = box(col_id).u; a != col_id; a = box(a).u) {
    for (BoxId b = box(a).l; b != a; b = box(b).l) {
      box(b).show_ud(*this);
      ++sizes[box(b).x];
    }
  }
  box(col_id).show_lr(*this);
}

Box& linked_matrix::create_box(unsigned x, unsigned y) {
  unsigned id = boxes.size();
  Box b{id, x, y};
  boxes.emplace_back(std::move(b));
  return boxes[boxes.size() - 1];
}
