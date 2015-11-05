#include "linked_matrix.hpp"

#include <algorithm>

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
  lm->root = new box;
  if (rows.empty()) {
    return lm;
  }

  for (auto& row : rows) {
    for (unsigned x : row) {
      if (x + 1 > width) {
	width = x + 1;
      }
    }
  }

  lm->cols.resize(width);
  for (unsigned x = 0; x < width; ++x) {
    box *col = new box;
    col->size = 0;
    lm->cols[x] = col;
    if (x >= secondary) {
      lm->root->link_l(col);
    }
  }
  for (unsigned i = 0; i < rows.size(); ++i) {
    auto& xs = rows[i];
    box *row = new box;
    for (unsigned x : xs) {
      if (x >= width) {
        return nullptr;
      }
      box *cell = new box;
      cell->x = x;
      cell->y = i;
      lm->cols[x]->link_u(cell);
      ++lm->cols[x]->size;
      row->link_l(cell);
    }
    row->hide_lr();
    delete row;
  }
  return lm;
}

void linked_matrix::cover_column(box *col) {
  col->hide_lr();
  for (box *row = col->d; row != col; row = row->d) {
    for (box *cell = row->r; cell != row; cell = cell->r) {
      cell->hide_ud();
      --cols[cell->x]->size;
    }
  }
}

void linked_matrix::uncover_column(box *col) {
  for (box *row = col->u; row != col; row = row->u) {
    for (box *cell = row->l; cell != row; cell = cell->l) {
      cell->show_ud();
      ++cols[cell->x]->size;
    }
  }
  col->show_lr();
}
