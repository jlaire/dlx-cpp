#pragma once

#include <memory>
#include <vector>

class GeneralizedLinkedMatrix {
public:
  using VVI = std::vector<std::vector<int>>;
  using Ptr = std::unique_ptr<GeneralizedLinkedMatrix>;
  using NodeId = unsigned;

  static Ptr make(unsigned width, const VVI& rows, unsigned secondary = 0);
  static Ptr make_from_dense_matrix(const VVI& rows, unsigned secondary = 0);

  void cover_column(NodeId id);
  void uncover_column(NodeId id);

  auto width() const -> unsigned;
  auto root_id() const -> NodeId;

  auto E(NodeId id) const -> bool;
  auto X(NodeId id) const -> unsigned;
  auto Y(NodeId id) const -> unsigned;
  auto S(NodeId id) const -> unsigned;
  auto C(NodeId id) const -> NodeId;
  auto L(NodeId id) const -> NodeId;
  auto R(NodeId id) const -> NodeId;
  auto U(NodeId id) const -> NodeId;
  auto D(NodeId id) const -> NodeId;

private:
  GeneralizedLinkedMatrix(unsigned width, const VVI& rows, unsigned secondary);
  void add_row(unsigned y, const std::vector<int>& xs);

  NodeId create_node(unsigned x, unsigned y, bool epsilon = false);

  struct Node;
  std::vector<NodeId> col_ids_;
  std::vector<unsigned> sizes_;
  std::vector<unsigned> covered_;
  std::vector<Node> nodes_;
};

struct GeneralizedLinkedMatrix::Node
{
  NodeId id;
  unsigned x, y;
  bool epsilon;
  NodeId l, r, u, d;
  explicit Node(NodeId id_, unsigned x_, unsigned y_, bool epsilon_)
    : id(id_), x(x_), y(y_),
    epsilon(epsilon_),
    l(id), r(id), u(id), d(id)
  {
  }
};

inline auto GeneralizedLinkedMatrix::width() const -> unsigned { return col_ids_.size(); }
inline auto GeneralizedLinkedMatrix::root_id() const -> NodeId { return 0; }
inline auto GeneralizedLinkedMatrix::E(NodeId id) const -> bool { return nodes_[id].epsilon; }
inline auto GeneralizedLinkedMatrix::X(NodeId id) const -> unsigned { return nodes_[id].x; }
inline auto GeneralizedLinkedMatrix::Y(NodeId id) const -> unsigned { return nodes_[id].y; }
inline auto GeneralizedLinkedMatrix::S(NodeId id) const -> unsigned { return sizes_[X(id)]; }
inline auto GeneralizedLinkedMatrix::C(NodeId id) const -> NodeId { return col_ids_[X(id)]; }
inline auto GeneralizedLinkedMatrix::L(NodeId id) const -> NodeId { return nodes_[id].l; }
inline auto GeneralizedLinkedMatrix::R(NodeId id) const -> NodeId { return nodes_[id].r; }
inline auto GeneralizedLinkedMatrix::U(NodeId id) const -> NodeId { return nodes_[id].u; }
inline auto GeneralizedLinkedMatrix::D(NodeId id) const -> NodeId { return nodes_[id].d; }
