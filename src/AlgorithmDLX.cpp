#include <dlx/AlgorithmDLX.hpp>

#include <assert.h>
#include <stdexcept>

AlgorithmDLX::AlgorithmDLX(const ExactCoverProblem& problem)
  : A_(problem)
{
}

auto AlgorithmDLX::count_solutions() -> unsigned {
  auto options = Options();
  options.get_solutions = false;
  return search(options).number_of_solutions;
}

auto AlgorithmDLX::find_solutions(unsigned max) -> std::vector<Solution> {
  auto options = Options();
  options.max_solutions = max;
  return search(options).solutions;
}

auto AlgorithmDLX::search() -> Result {
  return search({});
}

auto AlgorithmDLX::search(const Options& options) -> Result {
  if (options.choose_random_column && options.random_engine == nullptr) {
    throw std::runtime_error("Random engine must be given");
  }
  auto result = Result();
  auto state = SearchState();
  search(result, options, state);
  return result;
}

void AlgorithmDLX::search(Result& result, const Options& options, SearchState& state) {
  if (state.stopped) {
    return;
  }

  while (result.profile.size() <= state.stack.size()) {
    result.profile.push_back(0);
  }
  ++result.profile[state.stack.size()];

  auto h = A_.root_id();
  if (R(h) == h) {
    ++result.number_of_solutions;
    if (options.get_solutions) {
      result.solutions.push_back(state.stack);
    }
    if (result.number_of_solutions >= options.max_solutions) {
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
  assert(!cs.empty());
  if (S(cs[0]) < 1) {
    return;
  }

  auto c = cs[0];
  if (options.choose_random_column) {
    auto randint = std::uniform_int_distribution<unsigned>(0, cs.size() - 1);
    c = cs[randint(*options.random_engine)];
  }

  cover_column(c);
  for (auto r = D(c); r != c; r = D(r)) {
    state.stack.push_back(Y(r));
    for (auto j = R(r); j != r; j = R(j))
      cover_column(j);
    search(result, options, state);
    for (auto j = L(r); j != r; j = L(j))
      uncover_column(j);
    state.stack.pop_back();
  }
  uncover_column(c);
}
