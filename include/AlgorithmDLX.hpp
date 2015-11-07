#pragma once

#include "LinkedMatrix.hpp"

#include <stdint.h>
#include <functional>

class AlgorithmDLX {
public:
  using Solution = std::vector<unsigned>;
  using SolutionHandler = std::function<void(const Solution&)>;

  AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& A, SolutionHandler callback);

  void search();

private:
  std::unique_ptr<LinkedMatrix> A_;
  SolutionHandler callback_;
  std::vector<unsigned> O;

  // These make the implementation look just like in "Dancing Links".
  //
  // An alternative would be to implement `search()` in `LinkedMatrix`,
  // but clean semantics trumps clean syntax.

  using NodeId = LinkedMatrix::NodeId;
  void cover_column(NodeId id) { A_->cover_column(id); }
  void uncover_column(NodeId id) { A_->uncover_column(id); }

  unsigned row(NodeId id) { return A_->row(id); }
  unsigned S(NodeId id) { return A_->S(id); }
  NodeId L(NodeId id) { return A_->L(id); }
  NodeId R(NodeId id) { return A_->R(id); }
  NodeId U(NodeId id) { return A_->U(id); }
  NodeId D(NodeId id) { return A_->D(id); }
};
