#include "Langford.hpp"

#include <unistd.h>
#include <iostream>

int main(int argc, char **argv) {
  bool opt_verbose = false;
  for (int opt; (opt = ::getopt(argc, argv, "v")) != -1;) {
    if (opt == 'v') {
      opt_verbose = true;
    }
  }

  for (unsigned n; std::cin >> n;) {
    auto langford = Langford(n);
    std::cout
      << "Number of unique Langford pairings for n = " << n << ": "
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
