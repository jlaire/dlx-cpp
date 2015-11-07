#include "LinkedMatrix.hpp"

#include <algorithm>

LinkedMatrix::LinkedMatrix() {
  Node& root = create_node(~0, ~0);
  root_id = root.id;
}

std::unique_ptr<LinkedMatrix> LinkedMatrix::from_boolean_rows(
    const VectorVector& rows, unsigned secondary)
{
  VectorVector sparse(rows.size());
  for (unsigned i = 0; i < rows.size(); ++i) {
    for (unsigned j = 0; j < rows[i].size(); ++j) {
      if (rows[i][j]) {
        sparse[i].push_back(j);
      }
    }
  }
  return from_sparse_matrix(sparse, secondary, rows[0].size());
}

std::unique_ptr<LinkedMatrix> LinkedMatrix::from_sparse_matrix(
    const VectorVector& rows, unsigned secondary, unsigned width)
{
  std::unique_ptr<LinkedMatrix> lm{new LinkedMatrix};
  if (rows.empty()) {
    return lm;
  }

  for (auto& row : rows) {
    for (unsigned x : row) {
      width = std::max(width, x + 1);
    }
  }

  lm->col_ids.resize(width);
  lm->sizes.resize(width);
  for (unsigned x = 0; x < width; ++x) {
    lm->sizes[x] = 0;
    Node& col = lm->create_node(x, ~0);
    lm->col_ids[x] = col.id;
    if (x >= secondary) {
      lm->root().link_l(*lm, col);
    }
  }
  for (unsigned y = 0; y < rows.size(); ++y) {
    auto& xs = rows[y];
    NodeId row_id;
    {
      Node& row = lm->create_node(~0, y);
      row_id = row.id;
    }
    for (unsigned x : xs) {
      if (x >= width) {
        return nullptr;
      }
      Node& cell = lm->create_node(x, y);
      lm->node(lm->col_ids[x]).link_u(*lm, cell);
      ++lm->sizes[x];
      lm->node(row_id).link_l(*lm, cell);
    }
    lm->node(row_id).hide_lr(*lm);
  }
  return lm;
}

void LinkedMatrix::cover_column(unsigned x) {
  NodeId col_id = col_ids[x];
  node(col_id).hide_lr(*this);
  for (NodeId a = node(col_id).d; a != col_id; a = node(a).d) {
    for (NodeId b = node(a).r; b != a; b = node(b).r) {
      node(b).hide_ud(*this);
      --sizes[node(b).x];
    }
  }
}

void LinkedMatrix::uncover_column(unsigned x) {
  NodeId col_id = col_ids[x];
  for (NodeId a = node(col_id).u; a != col_id; a = node(a).u) {
    for (NodeId b = node(a).l; b != a; b = node(b).l) {
      node(b).show_ud(*this);
      ++sizes[node(b).x];
    }
  }
  node(col_id).show_lr(*this);
}

Node& LinkedMatrix::create_node(unsigned x, unsigned y) {
  unsigned id = nodes.size();
  Node b{id, x, y};
  nodes.emplace_back(std::move(b));
  return nodes[nodes.size() - 1];
}
