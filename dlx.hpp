#pragma once

#include "linked_matrix.hpp"

#include <stdint.h>
#include <functional>

struct dlx {
  using SolutionHandler = std::function<void(const std::vector<unsigned>&)>;
  bool print_running_count = false;

  explicit dlx(linked_matrix *lm, SolutionHandler sh)
    : lm(lm), solution_handler(std::move(sh))
  {
  }

  void solve();

private:
  linked_matrix *lm;
  SolutionHandler solution_handler;
  std::vector<unsigned> stack;
};
