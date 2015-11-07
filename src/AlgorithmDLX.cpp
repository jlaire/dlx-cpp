#include "AlgorithmDLX.hpp"

AlgorithmDLX::AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& A, SolutionHandler callback)
  : A_(std::move(A)), callback_(std::move(callback))
{
}

void AlgorithmDLX::search() {
  auto h = A_->root_id();
  if (A_->R(h) == h) {
    callback_(stack_);
    return;
  }

  unsigned min_size = ~0;
  auto c = h;
  for (auto j = A_->R(h); j != h; j = A_->R(j)) {
    unsigned size = A_->S(j);
    if (size < min_size) {
      min_size = size;
      c = j;
    }
  }
  if (min_size < 1) {
    return;
  }

  A_->cover_column(c);
  for (auto r = A_->D(c); r != c; r = A_->D(r)) {
    stack_.push_back(A_->row(r));
    for (auto j = A_->R(r); j != r; j = A_->R(j)) {
      A_->cover_column(j);
    }
    search();
    for (auto j = A_->L(r); j != r; j = A_->L(j)) {
      A_->uncover_column(j);
    }
    stack_.pop_back();
  }
  A_->uncover_column(c);
}
