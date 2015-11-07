#include "AlgorithmDLX.hpp"

void AlgorithmDLX::search() {
  auto col_id = lm->root().r;
  if (col_id == lm->root_id) {
    solution_handler(stack);
    return;
  }
  unsigned min_size = lm->sizes[lm->node(col_id).x];
  for (auto id = lm->node(col_id).r; id != lm->root_id; id = lm->node(id).r) {
    unsigned size = lm->sizes[lm->node(id).x];
    if (size < min_size) {
      min_size = size;
      col_id = id;
    }
  }
  if (min_size < 1) {
    return;
  }
  unsigned x = lm->node(col_id).x;
  lm->cover_column(x);
  for (auto id = lm->node(col_id).d; id != col_id; id = lm->node(id).d) {
    int y = lm->node(id).y;
    stack.push_back(y);
    for (auto a = lm->node(id).r; a != id; a = lm->node(a).r) {
      lm->cover_column(lm->node(a).x);
    }
    search();
    for (auto a = lm->node(id).l; a != id; a = lm->node(a).l) {
      lm->uncover_column(lm->node(a).x);
    }
    stack.pop_back();
  }
  lm->uncover_column(x);
}
