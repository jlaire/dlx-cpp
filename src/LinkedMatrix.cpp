#include "LinkedMatrix.hpp"

#include <assert.h>
#include <algorithm>

LinkedMatrix::LinkedMatrix() {
  NodeId id = create_node(~0, ~0);
  assert(id == root_id());
}

LinkedMatrix::NodeId LinkedMatrix::root_id() const { return 0; }
unsigned LinkedMatrix::row(NodeId id) const { return nodes_[id].y; }
unsigned LinkedMatrix::S(NodeId id) const { return sizes_[nodes_[id].x]; }
LinkedMatrix::NodeId LinkedMatrix::C(NodeId id) const { return col_ids_[nodes_[id].x]; }
LinkedMatrix::NodeId LinkedMatrix::L(NodeId id) const { return nodes_[id].l; }
LinkedMatrix::NodeId LinkedMatrix::R(NodeId id) const { return nodes_[id].r; }
LinkedMatrix::NodeId LinkedMatrix::U(NodeId id) const { return nodes_[id].u; }
LinkedMatrix::NodeId LinkedMatrix::D(NodeId id) const { return nodes_[id].d; }

std::unique_ptr<LinkedMatrix> LinkedMatrix::from_boolean_rows(
    const VectorVector& rows, unsigned secondary)
{
  unsigned width = rows.empty() ? 0 : rows[0].size();
  VectorVector sparse(rows.size());
  for (unsigned i = 0; i < rows.size(); ++i) {
    for (unsigned j = 0; j < rows[i].size(); ++j) {
      if (rows[i][j]) {
        sparse[i].push_back(j);
      }
    }
  }
  return from_sparse_matrix(sparse, secondary, width);
}

std::unique_ptr<LinkedMatrix> LinkedMatrix::from_sparse_matrix(
    const VectorVector& rows, unsigned secondary, unsigned width)
{
  std::unique_ptr<LinkedMatrix> lm{new LinkedMatrix};
  lm->initialize_from_sparse_matrix(rows, secondary, width);
  return lm;
}

void LinkedMatrix::initialize_from_sparse_matrix(
    const VectorVector& rows, unsigned secondary, unsigned width)
{
  if (rows.empty()) {
    return;
  }

  for (auto& row : rows) {
    for (unsigned x : row) {
      width = std::max(width, x + 1);
    }
  }

  col_ids_.resize(width);
  sizes_.resize(width);
  for (unsigned x = 0; x < width; ++x) {
    sizes_[x] = 0;
    NodeId col_id = create_node(x, ~0);
    col_ids_[x] = col_id;
    if (x >= secondary) {
      nodes_[root_id()].link_l(*this, nodes_[col_id]);
    }
  }
  for (unsigned y = 0; y < rows.size(); ++y) {
    auto& xs = rows[y];
    NodeId row_id = create_node(~0, y);
    for (unsigned x : xs) {
      if (x >= width) {
        throw "";
      }
      NodeId id = create_node(x, y);
      nodes_[col_ids_[x]].link_u(*this, nodes_[id]);
      ++sizes_[x];
      nodes_[row_id].link_l(*this, nodes_[id]);
    }
    nodes_[row_id].hide_lr(*this);
  }
}

void LinkedMatrix::cover_column(NodeId c) {
  c = C(c);
  nodes_[c].hide_lr(*this);
  for (NodeId i = D(c); i != c; i = D(i)) {
    for (NodeId j = R(i); j != i; j = R(j)) {
      nodes_[j].hide_ud(*this);
      --sizes_[nodes_[j].x];
    }
  }
}

void LinkedMatrix::uncover_column(NodeId c) {
  c = C(c);
  for (NodeId i = U(c); i != c; i = U(i)) {
    for (NodeId j = L(i); j != i; j = L(j)) {
      nodes_[j].show_ud(*this);
      ++sizes_[nodes_[j].x];
    }
  }
  nodes_[c].show_lr(*this);
}

LinkedMatrix::NodeId LinkedMatrix::create_node(unsigned x, unsigned y) {
  unsigned id = nodes_.size();
  nodes_.emplace_back(id, x, y);
  return id;
}
