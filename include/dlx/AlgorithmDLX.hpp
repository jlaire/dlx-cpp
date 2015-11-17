#pragma once

#include "LinkedMatrix.hpp"

#include <stdint.h>
#include <functional>

class AlgorithmDLX {
public:
  using Solution = std::vector<unsigned>;
  using Callback = std::function<bool(const Solution&)>;

  explicit AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& A);

  void search(Callback);
  auto count_solutions() -> unsigned;
  auto find_solutions() -> std::vector<Solution>;

private:
  std::unique_ptr<LinkedMatrix> A_;

  struct SearchState;
  void search(SearchState& state);

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

struct AlgorithmDLX::SearchState
{
  explicit SearchState(Callback callback)
    : callback(std::move(callback))
  {
  }

  Callback callback;
  std::vector<unsigned> stack;
  bool stopped = false;
};
