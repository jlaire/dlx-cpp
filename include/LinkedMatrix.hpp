#pragma once

#include <memory>
#include <vector>

struct Node;

struct LinkedMatrix {
  using VectorVector = std::vector<std::vector<unsigned>>;
  using NodeId = unsigned;

  NodeId root_id;
  std::vector<NodeId> col_ids;
  std::vector<unsigned> sizes;

  static std::unique_ptr<LinkedMatrix> from_boolean_rows(const VectorVector& rows, unsigned secondary);
  static std::unique_ptr<LinkedMatrix> from_sparse_matrix(const VectorVector& rows, unsigned secondary, unsigned width = 0);

  void cover_column(unsigned x);
  void uncover_column(unsigned x);

  Node& node(NodeId id) { return nodes[id]; }
  Node& operator[](NodeId id) { return nodes[id]; }
  Node& root() { return nodes[root_id]; }

private:
  LinkedMatrix();
  Node& create_node(unsigned x, unsigned y);

  std::vector<Node> nodes;
};

struct Node {
  using NodeId = LinkedMatrix::NodeId;
  NodeId id;

  unsigned x, y;
  NodeId l, r, u, d;
  explicit Node(NodeId id_, unsigned x_, unsigned y_)
    : id(id_), x(x_), y(y_),
    l(id), r(id), u(id), d(id)
  {
  }

  void hide_lr(LinkedMatrix& M) { M[l].r = r, M[r].l = l; }
  void show_lr(LinkedMatrix& M) { M[l].r = M[r].l = id; }
  void hide_ud(LinkedMatrix& M) { M[u].d = d, M[d].u = u; }
  void show_ud(LinkedMatrix& M) { M[u].d = M[d].u = id; }
  void link_l(LinkedMatrix& M, Node& b) { b.r = id, b.l = l; l = M[l].r = b.id; }
  void link_u(LinkedMatrix& M, Node& b) { b.d = id, b.u = u; u = M[u].d = b.id; }
};
