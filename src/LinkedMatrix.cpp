#include <dlx/LinkedMatrix.hpp>

#include <assert.h>
#include <algorithm>

LinkedMatrix::LinkedMatrix() {
  NodeId id = create_node(~0, ~0);
  assert(id == root_id());
}

unsigned LinkedMatrix::S(NodeId id) const { return sizes_[nodes_[id].x]; }
auto LinkedMatrix::root_id() const -> NodeId { return 0; }
auto LinkedMatrix::C(NodeId id) const -> NodeId { return col_ids_[nodes_[id].x]; }
auto LinkedMatrix::L(NodeId id) const -> NodeId { return nodes_[id].l; }
auto LinkedMatrix::R(NodeId id) const -> NodeId { return nodes_[id].r; }
auto LinkedMatrix::U(NodeId id) const -> NodeId { return nodes_[id].u; }
auto LinkedMatrix::D(NodeId id) const -> NodeId { return nodes_[id].d; }

std::unique_ptr<LinkedMatrix> LinkedMatrix::from_dense_matrix(
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
  return from_sparse_matrix(width, sparse, secondary);
}

std::unique_ptr<LinkedMatrix> LinkedMatrix::from_sparse_matrix(
    unsigned width, const VectorVector& rows, unsigned secondary)
{
  std::unique_ptr<LinkedMatrix> lm{new LinkedMatrix};
  lm->initialize_from_sparse_matrix(width, rows, secondary);
  return lm;
}

void LinkedMatrix::initialize_from_sparse_matrix(
    unsigned width, const VectorVector& rows, unsigned secondary)
{
  if (secondary > width) {
    throw "";
  }
  for (auto& row : rows) {
    for (unsigned x : row) {
      if (x >= width) {
        throw "";
      }
    }
  }

  col_ids_ = std::vector<NodeId>(width);
  sizes_ = std::vector<unsigned>(width);
  for (unsigned x = 0; x < width; ++x) {
    NodeId col_id = create_node(x, ~0);
    col_ids_[x] = col_id;
    if (x >= secondary) {
      nodes_[root_id()].link_l(*this, nodes_[col_id]);
    }
  }

  for (unsigned y = 0; y < rows.size(); ++y) {
    if (rows[y].empty()) {
      continue;
    }

    NodeId first_id = 0;
    for (auto x : rows[y]) {
      NodeId id = create_node(x, y);
      nodes_[col_ids_[x]].link_u(*this, nodes_[id]);
      ++sizes_[x];
      if (first_id == 0) {
	first_id = id;
      }
      else {
	nodes_[first_id].link_l(*this, nodes_[id]);
      }
    }
  }
}

void LinkedMatrix::cover_column(NodeId c) {
  c = C(c);
  nodes_[c].hide_lr(*this);
  for (NodeId i = D(c); i != c; i = D(i)) {
    for (NodeId j = R(i); j != i; j = R(j)) {
      nodes_[j].hide_ud(*this);
      --sizes_[X(j)];
    }
  }
}

void LinkedMatrix::uncover_column(NodeId c) {
  c = C(c);
  for (NodeId i = U(c); i != c; i = U(i)) {
    for (NodeId j = L(i); j != i; j = L(j)) {
      nodes_[j].show_ud(*this);
      ++sizes_[X(j)];
    }
  }
  nodes_[c].show_lr(*this);
}

LinkedMatrix::NodeId LinkedMatrix::create_node(unsigned x, unsigned y) {
  unsigned id = nodes_.size();
  nodes_.emplace_back(id, x, y);
  return id;
}
