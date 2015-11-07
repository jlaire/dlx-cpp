#pragma once

#include "LinkedMatrix.hpp"

#include <stdint.h>
#include <functional>

class AlgorithmDLX {
public:
  using Solution = std::vector<unsigned>;
  using SolutionHandler = std::function<void(const Solution&)>;

  AlgorithmDLX(std::unique_ptr<LinkedMatrix>&& A, SolutionHandler callback);

  void search();

private:
  std::unique_ptr<LinkedMatrix> A_;
  SolutionHandler callback_;
  std::vector<unsigned> stack_;
};
