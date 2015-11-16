#include <dlx/LinkedMatrix.hpp>

#include <assert.h>
#include <algorithm>

LinkedMatrix::LinkedMatrix() {
  NodeId id = create_node(~0, ~0);
  assert(id == root_id());
}

auto LinkedMatrix::make_from_dense_matrix(const VVU& rows, unsigned secondary)
  -> Ptr
{
  unsigned width = rows.empty() ? 0 : rows[0].size();
  VVU sparse(rows.size());
  for (unsigned i = 0; i < rows.size(); ++i) {
    if (rows[i].size() != width) {
      throw "";
    }
    for (unsigned j = 0; j < rows[i].size(); ++j) {
      if (rows[i][j]) {
        sparse[i].push_back(j);
      }
    }
  }
  return make(width, sparse, secondary);
}

auto LinkedMatrix::make(unsigned width, const VVU& rows, unsigned secondary)
  -> Ptr
{
  auto lm = std::unique_ptr<LinkedMatrix>(new LinkedMatrix);
  lm->initialize(width, rows, secondary);
  return lm;
}

void LinkedMatrix::initialize(
    unsigned width, const VVU& rows, unsigned secondary)
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

  auto link_l = [&](NodeId a, NodeId b) {
    nodes_[b].r = a;
    nodes_[b].l = L(a);
    nodes_[L(a)].r = b;
    nodes_[a].l = b;
  };

  col_ids_ = std::vector<NodeId>(width);
  sizes_ = std::vector<unsigned>(width);
  for (unsigned x = 0; x < width; ++x) {
    NodeId col_id = create_node(x, ~0);
    col_ids_[x] = col_id;
    if (x >= secondary) {
      link_l(root_id(), col_id);
    }
  }

  for (unsigned y = 0; y < rows.size(); ++y) {
    if (rows[y].empty()) {
      continue;
    }

    NodeId first_id = 0;
    for (auto x : rows[y]) {
      NodeId id = create_node(x, y);
      nodes_[id].d = C(id);
      nodes_[id].u = U(C(id));
      nodes_[U(C(id))].d = id;
      nodes_[C(id)].u = id;
      ++sizes_[x];
      if (first_id == 0) {
        first_id = id;
      }
      else {
        link_l(first_id, id);
      }
    }
  }
}

void LinkedMatrix::cover_column(NodeId c) {
  c = C(c);
  nodes_[L(c)].r = R(c);
  nodes_[R(c)].l = L(c);
  for (NodeId i = D(c); i != c; i = D(i)) {
    for (NodeId j = R(i); j != i; j = R(j)) {
      nodes_[U(j)].d = D(j);
      nodes_[D(j)].u = U(j);
      --sizes_[X(j)];
    }
  }
}

void LinkedMatrix::uncover_column(NodeId c) {
  c = C(c);
  for (NodeId i = U(c); i != c; i = U(i)) {
    for (NodeId j = L(i); j != i; j = L(j)) {
      nodes_[U(j)].d = j;
      nodes_[D(j)].u = j;
      ++sizes_[X(j)];
    }
  }
  nodes_[L(c)].r = c;
  nodes_[R(c)].l = c;
}

LinkedMatrix::NodeId LinkedMatrix::create_node(unsigned x, unsigned y) {
  unsigned id = nodes_.size();
  nodes_.emplace_back(id, x, y);
  return id;
}
