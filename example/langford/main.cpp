#include "Langford.hpp"

#include <iostream>

int main() {
  for (auto n = 0u; std::cin >> n;) {
    std::cout
      << "Langford pairs for n = " << n << ": "
      << Langford::count_solutions(n) << '\n';
  }
}
