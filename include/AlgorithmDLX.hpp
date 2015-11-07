#pragma once

#include "LinkedMatrix.hpp"

#include <stdint.h>
#include <functional>

struct AlgorithmDLX {
  using Solution = std::vector<unsigned>;
  using SolutionHandler = std::function<void(const Solution&)>;

  AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& lm, SolutionHandler sh)
    : lm(std::move(lm)), solution_handler(std::move(sh))
  {
  }

  void search();

private:
  std::unique_ptr<LinkedMatrix> lm;
  SolutionHandler solution_handler;
  std::vector<unsigned> stack;
};
