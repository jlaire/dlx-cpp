#pragma once

#include "linked_matrix.hpp"

#include <stdint.h>
#include <functional>

struct dlx {
  using SolutionHandler = std::function<void(const std::vector<unsigned>&)>;

  explicit dlx(std::unique_ptr<linked_matrix>&& lm, SolutionHandler sh)
    : lm(std::move(lm)), solution_handler(std::move(sh))
  {
  }

  void solve();

private:
  std::unique_ptr<linked_matrix> lm;
  SolutionHandler solution_handler;
  std::vector<unsigned> stack;
};
