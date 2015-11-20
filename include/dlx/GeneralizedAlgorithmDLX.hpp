#pragma once

#include "GeneralizedLinkedMatrix.hpp"

#include <stdint.h>
#include <functional>

class GeneralizedAlgorithmDLX {
public:
  using Solution = std::vector<unsigned>;
  using SolutionCallback = std::function<bool(const Solution&)>;
  using NodeCallback = std::function<void(bool, const Solution&)>;

  explicit GeneralizedAlgorithmDLX(std::unique_ptr<GeneralizedLinkedMatrix>&& A);

  void search(SolutionCallback);
  auto count_solutions() -> unsigned;
  auto find_solutions() -> std::vector<Solution>;
  auto get_nodes_per_depth() -> std::vector<unsigned>;

private:
  std::unique_ptr<GeneralizedLinkedMatrix> A_;

  struct SearchState;
  void search(SearchState& state);

  using NodeId = GeneralizedLinkedMatrix::NodeId;
  void cover_column(NodeId id) { A_->cover_column(id); }
  void uncover_column(NodeId id) { A_->uncover_column(id); }
  auto Y(NodeId id) -> unsigned { return A_->Y(id); }
  auto S(NodeId id) -> unsigned { return A_->S(id); }
  auto L(NodeId id) -> NodeId { return A_->L(id); }
  auto R(NodeId id) -> NodeId { return A_->R(id); }
  auto U(NodeId id) -> NodeId { return A_->U(id); }
  auto D(NodeId id) -> NodeId { return A_->D(id); }
};

struct GeneralizedAlgorithmDLX::SearchState
{
  explicit SearchState(SolutionCallback sc)
    : SearchState(std::move(sc), NodeCallback())
  {
  }

  SearchState(SolutionCallback sc, NodeCallback nc)
    : solution_callback(std::move(sc)),
    node_callback(std::move(nc))
  {
  }

  SolutionCallback solution_callback;
  NodeCallback node_callback;
  std::vector<unsigned> stack;
  bool stopped = false;
};
