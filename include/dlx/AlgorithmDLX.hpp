#pragma once

#include "LinkedMatrix.hpp"

#include <stdint.h>
#include <functional>
#include <limits>
#include <random>

class AlgorithmDLX {
public:
  using Solution = std::vector<unsigned>;

  struct Options
  {
    bool choose_random_column = false;
    bool get_solutions = true;
    unsigned max_solutions = std::numeric_limits<unsigned>::max();
    std::mt19937 *random_engine = nullptr;
  };

  struct Result
  {
    unsigned number_of_solutions = 0;
    std::vector<Solution> solutions;
    std::vector<unsigned> profile;
  };

  explicit AlgorithmDLX(const ExactCoverProblem& problem);

  auto search() -> Result;
  auto search(const Options&) -> Result;
  auto count_solutions() -> unsigned;
  auto find_solutions(unsigned max = ~0u) -> std::vector<Solution>;

private:
  LinkedMatrix A_;

  struct SearchState;
  void search(Result& result, const Options& options, SearchState& state);

  using NodeId = LinkedMatrix::NodeId;
  void cover_column(NodeId id) { A_.cover_column(id); }
  void uncover_column(NodeId id) { A_.uncover_column(id); }
  auto Y(NodeId id) -> unsigned { return A_.Y(id); }
  auto S(NodeId id) -> unsigned { return A_.S(id); }
  auto L(NodeId id) -> NodeId { return A_.L(id); }
  auto R(NodeId id) -> NodeId { return A_.R(id); }
  auto U(NodeId id) -> NodeId { return A_.U(id); }
  auto D(NodeId id) -> NodeId { return A_.D(id); }
};

struct AlgorithmDLX::SearchState
{
  std::vector<unsigned> stack;
  bool stopped = false;
};
