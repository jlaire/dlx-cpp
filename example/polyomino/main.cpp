#include "Polyomino.hpp"

#include <dlx/AlgorithmDLX.hpp>

int main() {
  auto pentomino = Polyomino();
  auto problem = pentomino.problem();
  auto result = AlgorithmDLX(problem).search();
  for (const auto& used_rows : result.solutions) {
    auto solution = pentomino.make_solution(used_rows);
    for (const auto& line : solution) {
      std::cout << line << '\n';
    }
    std::cout << '\n';
  }
  std::cout << result.number_of_solutions << '\n';
}
