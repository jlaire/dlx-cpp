#include <dlx/AlgorithmDLX.hpp>

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

auto AlgorithmDLX::find_solutions() -> std::vector<Solution> {
  auto solutions = std::vector<Solution>();
  search([&](const Solution& solution) -> bool {
    solutions.push_back(solution);
    return false;
  });
  return solutions;
}

void AlgorithmDLX::search(Callback callback) {
  auto state = SearchState(std::move(callback));
  search(state);
}

void AlgorithmDLX::search(SearchState& state) {
  if (state.stopped) {
    return;
  }
  auto h = A_->root_id();
  if (R(h) == h) {
    if (state.callback(state.stack)) {
      state.stopped = true;
    }
    return;
  }

  unsigned min_size = ~0;
  auto c = h;
  for (auto j = R(h); j != h; j = R(j)) {
    if (S(j) < min_size) {
      min_size = S(j);
      c = j;
    }
  }
  if (min_size < 1) {
    return;
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
