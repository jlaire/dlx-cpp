#pragma once

#include <memory>
#include <vector>

struct Box;

struct linked_matrix {
  using VectorVector = std::vector<std::vector<unsigned>>;
  using BoxId = unsigned;

  BoxId root_id;
  std::vector<BoxId> col_ids;
  std::vector<unsigned> sizes;

  static std::unique_ptr<linked_matrix> from_boolean_rows(const VectorVector& rows, unsigned secondary);
  static std::unique_ptr<linked_matrix> from_sparse_matrix(const VectorVector& rows, unsigned secondary, unsigned width = 0);

  void cover_column(unsigned x);
  void uncover_column(unsigned x);

  Box& box(BoxId id) { return boxes[id]; }
  Box& operator[](BoxId id) { return boxes[id]; }
  Box& root() { return boxes[root_id]; }

private:
  std::vector<Box> boxes;

  Box& create_box(unsigned x, unsigned y);
};

struct Box {
  using BoxId = linked_matrix::BoxId;
  BoxId id;

  unsigned x, y;
  BoxId l, r, u, d;
  explicit Box(BoxId id_, unsigned x_, unsigned y_)
    : id(id_), x(x_), y(y_),
    l(id), r(id), u(id), d(id)
  {
  }

  void hide_lr(linked_matrix& M) { M[l].r = r, M[r].l = l; }
  void show_lr(linked_matrix& M) { M[l].r = M[r].l = id; }
  void hide_ud(linked_matrix& M) { M[u].d = d, M[d].u = u; }
  void show_ud(linked_matrix& M) { M[u].d = M[d].u = id; }
  void link_l(linked_matrix& M, Box& b) { b.r = id, b.l = l; l = M[l].r = b.id; }
  void link_u(linked_matrix& M, Box& b) { b.d = id, b.u = u; u = M[u].d = b.id; }
};
