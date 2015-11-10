#include <dlx/AlgorithmDLX.hpp>

AlgorithmDLX::AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& A, SolutionHandler callback)
  : A_(std::move(A)), callback_(std::move(callback))
{
}

void AlgorithmDLX::search() {
  if (stop_) {
    return;
  }
  auto h = A_->root_id();
  if (R(h) == h) {
    if (callback_(O)) {
      stop_ = true;
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
    O.push_back(row(r));
    for (auto j = R(r); j != r; j = R(j))
      cover_column(j);
    search();
    for (auto j = L(r); j != r; j = L(j))
      uncover_column(j);
    O.pop_back();
  }
  uncover_column(c);
}
