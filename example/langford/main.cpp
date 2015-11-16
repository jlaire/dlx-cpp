#include "Langford.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
  bool opt_verbose = false;
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == std::string("-v")) {
      opt_verbose = true;
      continue;
    }

    auto n = static_cast<unsigned>(std::stoi(argv[i]));
    auto langford = Langford(n);
    std::cout
      << "Solutions for n=" << n << ": "
      << langford.count_solutions() << '\n';
    if (opt_verbose) {
      for (const auto& solution : langford.find_solutions()) {
        for (auto i = 0u; i < solution.size(); ++i) {
          std::cout << solution[i] << " \n"[i == solution.size() - 1];
        }
      }
    }
  }
}
