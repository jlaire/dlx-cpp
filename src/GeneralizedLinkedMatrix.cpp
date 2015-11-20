#include <dlx/GeneralizedLinkedMatrix.hpp>

#include <assert.h>
#include <algorithm>

GeneralizedLinkedMatrix::GeneralizedLinkedMatrix(unsigned width, const VVI& rows, unsigned secondary)
  : col_ids_(width), sizes_(width), covered_(width)
{
  assert(secondary <= width);

  NodeId root = create_node(~0, ~0);
  assert(root == root_id());

  for (auto x = 0u; x < width; ++x) {
    NodeId id = create_node(x, ~0);
    col_ids_[x] = id;
    if (x >= secondary) {
      nodes_[id].r = root;
      nodes_[id].l = L(root);
      nodes_[L(root)].r = id;
      nodes_[root].l = id;
    }
  }

  for (auto y = 0u; y < rows.size(); ++y) {
    add_row(y, rows[y]);
  }
}

auto GeneralizedLinkedMatrix::make(unsigned width, const VVI& rows, unsigned secondary) -> Ptr {
  return std::unique_ptr<GeneralizedLinkedMatrix>(new GeneralizedLinkedMatrix(width, rows, secondary));
}

auto GeneralizedLinkedMatrix::make_from_dense_matrix(const VVI& rows, unsigned secondary) -> Ptr {
  unsigned width = rows.empty() ? 0 : rows[0].size();
  VVI sparse(rows.size());
  for (unsigned i = 0; i < rows.size(); ++i) {
    if (rows[i].size() != width) {
      throw "";
    }
    for (unsigned j = 0; j < rows[i].size(); ++j) {
      if (rows[i][j] != 0) {
        sparse[i].push_back(rows[i][j] >= 0 ? int(j) : -int(j));
      }
    }
  }
  return make(width, sparse, secondary);
}

void GeneralizedLinkedMatrix::add_row(unsigned y, const std::vector<int>& xs) {
  // TODO: What if xs isn't sorted?
  NodeId first_id = 0;
  for (auto x : xs) {
    bool epsilon = x < 0;
    x = abs(x);
    NodeId id = create_node(x, y, epsilon);
    nodes_[id].d = C(id);
    nodes_[id].u = U(C(id));
    nodes_[U(C(id))].d = id;
    nodes_[C(id)].u = id;
    ++sizes_[x];
    if (first_id == 0) {
      first_id = id;
    }
    else {
      nodes_[id].r = first_id;
      nodes_[id].l = L(first_id);
      nodes_[L(first_id)].r = id;
      nodes_[first_id].l = id;
    }
  }
}

void GeneralizedLinkedMatrix::cover_column(NodeId c) {
  if (covered_[X(c)]) {
    return;
  }
  covered_[X(c)] = 1;
  bool epsilon = E(c);
  c = C(c);
  nodes_[L(c)].r = R(c);
  nodes_[R(c)].l = L(c);
  for (NodeId i = D(c); i != c; i = D(i)) {
    if (epsilon && E(i)) {
      continue;
    }
    for (NodeId j = R(i); j != i; j = R(j)) {
      nodes_[U(j)].d = D(j);
      nodes_[D(j)].u = U(j);
      --sizes_[X(j)];
    }
  }
}

void GeneralizedLinkedMatrix::uncover_column(NodeId c) {
  if (!covered_[X(c)]) {
    return;
  }
  covered_[X(c)] = 0;
  bool epsilon = E(c);
  c = C(c);
  for (NodeId i = U(c); i != c; i = U(i)) {
    if (epsilon && E(i)) {
      continue;
    }
    for (NodeId j = L(i); j != i; j = L(j)) {
      nodes_[U(j)].d = j;
      nodes_[D(j)].u = j;
      ++sizes_[X(j)];
    }
  }
  nodes_[L(c)].r = c;
  nodes_[R(c)].l = c;
}

GeneralizedLinkedMatrix::NodeId GeneralizedLinkedMatrix::create_node(unsigned x, unsigned y, bool epsilon) {
  assert(x <= width() || x == ~0u);
  unsigned id = nodes_.size();
  nodes_.emplace_back(id, x, y, epsilon);
  return id;
}
