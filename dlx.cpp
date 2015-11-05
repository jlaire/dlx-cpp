#include "dlx.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

void dlx::solve() {
  if (lm->root->r == lm->root) {
    solution_handler(stack);
    return;
  }
  box *col = lm->root->r;
  int min_size = col->size;
  for (box *c = col->r; c != lm->root; c = c->r) {
    if (c->size < min_size) {
      min_size = c->size;
      col = c;
    }
  }
  if (min_size < 1) {
    return;
  }
  lm->cover_column(col);
  for (box *row = col->d; row != col; row = row->d) {
    stack.push_back(row->y);
    for (box *cell = row->r; cell != row; cell = cell->r) {
      lm->cover_column(lm->cols[cell->x]);
    }
    solve();
    for (box *cell = row->l; cell != row; cell = cell->l) {
      lm->uncover_column(lm->cols[cell->x]);
    }
    stack.pop_back();
  }
  lm->uncover_column(col);
}
