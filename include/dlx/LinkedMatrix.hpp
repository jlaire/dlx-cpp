#pragma once

#include <memory>
#include <vector>

class LinkedMatrix {
public:
  using VVU = std::vector<std::vector<unsigned>>;
  using Ptr = std::unique_ptr<LinkedMatrix>;
  using NodeId = unsigned;

  static Ptr make(unsigned width, const VVU& rows, unsigned secondary = 0);
  static Ptr make_from_dense_matrix(const VVU& rows, unsigned secondary = 0);

  void cover_column(NodeId id);
  void uncover_column(NodeId id);

  NodeId root_id() const;

  auto X(NodeId id) const -> unsigned;
  auto Y(NodeId id) const -> unsigned;
  auto S(NodeId id) const -> unsigned;
  auto C(NodeId id) const -> NodeId;
  auto L(NodeId id) const -> NodeId;
  auto R(NodeId id) const -> NodeId;
  auto U(NodeId id) const -> NodeId;
  auto D(NodeId id) const -> NodeId;

private:
  LinkedMatrix();
  void initialize(unsigned width, const VVU& rows, unsigned secondary);
  void add_row(unsigned y, const std::vector<unsigned>& xs);

  NodeId create_node(unsigned x, unsigned y);

  struct Node;
  std::vector<NodeId> col_ids_;
  std::vector<unsigned> sizes_;
  std::vector<Node> nodes_;

  struct Node
  {
    NodeId id;
    unsigned x, y;
    NodeId l, r, u, d;
    explicit Node(NodeId id_, unsigned x_, unsigned y_)
      : id(id_), x(x_), y(y_),
      l(id), r(id), u(id), d(id)
    {
    }
  };
};

inline auto LinkedMatrix::root_id() const -> NodeId { return 0; }
inline auto LinkedMatrix::X(NodeId id) const -> unsigned { return nodes_[id].x; }
inline auto LinkedMatrix::Y(NodeId id) const -> unsigned { return nodes_[id].y; }
inline auto LinkedMatrix::S(NodeId id) const -> unsigned { return sizes_[X(id)]; }
inline auto LinkedMatrix::C(NodeId id) const -> NodeId { return col_ids_[X(id)]; }
inline auto LinkedMatrix::L(NodeId id) const -> NodeId { return nodes_[id].l; }
inline auto LinkedMatrix::R(NodeId id) const -> NodeId { return nodes_[id].r; }
inline auto LinkedMatrix::U(NodeId id) const -> NodeId { return nodes_[id].u; }
inline auto LinkedMatrix::D(NodeId id) const -> NodeId { return nodes_[id].d; }
