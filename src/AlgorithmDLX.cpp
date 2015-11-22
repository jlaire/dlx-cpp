#include <dlx/AlgorithmDLX.hpp>

#include <stdlib.h>

AlgorithmDLX::AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& A)
  : A_(std::move(A))
{
}

unsigned AlgorithmDLX::count_solutions() {
  auto count = 0u;
  search([&](const Solution& solution) -> bool {
    ++count;
    return false;
  });
  return count;
}

auto AlgorithmDLX::find_solutions(unsigned max) -> std::vector<Solution> {
  auto solutions = std::vector<Solution>();
  search([&](const Solution& solution) -> bool {
    solutions.push_back(solution);
    return solutions.size() >= max;
  });
  return solutions;
}

auto AlgorithmDLX::find_random_solution() -> Solution {
  auto solutions = std::vector<Solution>();
  auto state = SearchState(
    [&](const Solution& solution) -> bool {
      solutions.push_back(solution);
      return true;
    }
  );
  state.random_column = true;
  search(state);
  if (solutions.empty()) {
    throw std::runtime_error("");
  }
  return solutions[0];
}

auto AlgorithmDLX::get_nodes_per_depth() -> std::vector<unsigned> {
  auto widths = std::vector<unsigned>();
  auto state = SearchState(
    [](const Solution&) { return false; },
    [&](bool, const Solution& solution) {
      while (widths.size() <= solution.size()) {
        widths.push_back(0);
      }
      ++widths[solution.size()];
    }
  );
  search(state);
  return widths;
}

void AlgorithmDLX::search(SolutionCallback solution_callback) {
  auto state = SearchState(std::move(solution_callback));
  search(state);
}

void AlgorithmDLX::search(SearchState& state) {
  if (state.stopped) {
    return;
  }
  auto h = A_->root_id();
  auto is_solved = R(h) == h;
  if (state.node_callback) {
    state.node_callback(is_solved, state.stack);
  }
  if (is_solved) {
    if (state.solution_callback(state.stack)) {
      state.stopped = true;
    }
    return;
  }

  auto cs = std::vector<NodeId>();
  for (auto j = R(h); j != h; j = R(j)) {
    if (!cs.empty() && S(j) < S(cs[0])) {
      cs.clear();
    }
    if (cs.empty() || S(j) == S(cs[0])) {
      cs.push_back(j);
    }
  }
  if (S(cs[0]) < 1) {
    return;
  }

  auto c = cs[0];
  if (state.random_column) {
    static bool init = (::srand(::time(0)), true);
    (void)init;
    c = cs[::rand() % cs.size()];
  }

  cover_column(c);
  for (auto r = D(c); r != c; r = D(r)) {
    state.stack.push_back(Y(r));
    for (auto j = R(r); j != r; j = R(j))
      cover_column(j);
    search(state);
    for (auto j = L(r); j != r; j = L(j))
      uncover_column(j);
    state.stack.pop_back();
  }
  uncover_column(c);
}
