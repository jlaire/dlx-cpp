#pragma once

#include "LinkedMatrix.hpp"

#include <stdint.h>
#include <functional>

class AlgorithmDLX {
public:
  using Solution = std::vector<unsigned>;
  using SolutionHandler = std::function<bool(const Solution&)>;

  AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& A, SolutionHandler callback);

  void search();

private:
  std::unique_ptr<LinkedMatrix> A_;
  SolutionHandler callback_;
  std::vector<unsigned> O;
  bool stop_ = false;

  using NodeId = LinkedMatrix::NodeId;
  void cover_column(NodeId id) { A_->cover_column(id); }
  void uncover_column(NodeId id) { A_->uncover_column(id); }
  auto Y(NodeId id) -> unsigned { return A_->Y(id); }
  auto S(NodeId id) -> unsigned { return A_->S(id); }
  auto L(NodeId id) -> NodeId { return A_->L(id); }
  auto R(NodeId id) -> NodeId { return A_->R(id); }
  auto U(NodeId id) -> NodeId { return A_->U(id); }
  auto D(NodeId id) -> NodeId { return A_->D(id); }
};
