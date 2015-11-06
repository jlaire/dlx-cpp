#include "dlx.hpp"

void dlx::solve() {
  Box::BoxId col_id = lm->root().r;
  if (col_id == lm->root_id) {
    solution_handler(stack);
    return;
  }
  unsigned min_size = lm->sizes[lm->box(col_id).x];
  for (Box::BoxId id = lm->box(col_id).r; id != lm->root_id; id = lm->box(id).r) {
    unsigned size = lm->sizes[lm->box(id).x];
    if (size < min_size) {
      min_size = size;
      col_id = id;
    }
  }
  if (min_size < 1) {
    return;
  }
  int x = lm->box(col_id).x;
  lm->cover_column(x);
  for (Box::BoxId id = lm->box(col_id).d; id != col_id; id = lm->box(id).d) {
    int y = lm->box(id).y;
    stack.push_back(y);
    for (Box::BoxId a = lm->box(id).r; a != id; a = lm->box(a).r) {
      lm->cover_column(lm->box(a).x);
    }
    solve();
    for (Box::BoxId a = lm->box(id).l; a != id; a = lm->box(a).l) {
      lm->uncover_column(lm->box(a).x);
    }
    stack.pop_back();
  }
  lm->uncover_column(x);
}
