#pragma once

#include <memory>
#include <vector>

struct box {
  int x, y, size;
  box *l, *r, *u, *d;
  box() {
    l = r = u = d = this;
  }
  void hide_lr() { l->r = r, r->l = l; }
  void show_lr() { l->r = r->l = this; }
  void hide_ud() { u->d = d, d->u = u; }
  void show_ud() { u->d = d->u = this; }
  void link_l(box *b) { b->r = this, b->l = l; l = l->r = b; }
  void link_u(box *b) { b->d = this, b->u = u; u = u->d = b; }
};

struct linked_matrix {
  using VectorVector = std::vector<std::vector<unsigned>>;

  box *root;
  std::vector<box *> cols;

  static std::unique_ptr<linked_matrix> from_boolean_rows(const VectorVector& rows, unsigned secondary);
  static std::unique_ptr<linked_matrix> from_sparse_matrix(const VectorVector& rows, unsigned secondary, unsigned width = 0);

  void cover_column(box *col);
  void uncover_column(box *col);
};
