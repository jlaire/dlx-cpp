#include <dlx/AlgorithmDLX.hpp>

#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

template <typename T>
void print_range(const T& xs) {
  auto it = std::begin(xs);
  auto end = std::end(xs);
  for (; it != end;) {
    std::cout << *it;
    std::cout << " \n"[++it == end];
  }
}

}

int main(int argc, char *argv[]) {
  bool opt_print_solutions = false;
  bool opt_verbose = false;
  bool opt_sparse = false;
  bool opt_running_count = false;

  for (int opt; (opt = getopt(argc, argv, "pvsr")) != -1;) {
    switch (opt) {
    case 'p':
      opt_print_solutions = true;
      break;
    case 'v':
      opt_verbose = true;
      opt_print_solutions = true;
      break;
    case 's':
      opt_sparse = true;
      break;
    case 'r':
      opt_running_count = true;
      break;
    default:
      std::cerr << "Bug in getopt loop! Unexpected char: " << opt << '\n';
      return 1;
    }
  }

  unsigned width = 0;
  unsigned secondary_columns = 0;
  {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream ss(line);
    ss >> width >> secondary_columns;
  }

  std::vector<std::vector<unsigned>> input_rows;
  for (std::string line; std::getline(std::cin, line);) {
    std::istringstream ss(line);

    std::vector<unsigned> row;
    if (opt_sparse) {
      for (unsigned x; ss >> x;) {
        row.push_back(x);
      }
      std::sort(row.begin(), row.end());
    }
    else {
      row.resize(width);
      for (unsigned& x : row) {
        ss >> x;
      }
    }
    if (!row.empty()) {
      input_rows.emplace_back(row);
    }
  }

  std::unique_ptr<LinkedMatrix> lm;
  if (opt_sparse) {
    lm = LinkedMatrix::make(width, input_rows, secondary_columns);
  }
  else {
    lm = LinkedMatrix::make_from_dense_matrix(input_rows, secondary_columns);
  }

  uint64_t solution_count = 0;
  auto callback = [&](const std::vector<unsigned>& row_indices) -> bool {
    if (opt_print_solutions) {
      if (opt_verbose) {
        for (unsigned i : row_indices) {
          print_range(input_rows[i]);
        }
        std::cout << '\n';
      }
      else {
        print_range(row_indices);
      }
    }

    ++solution_count;
    if (opt_running_count && (solution_count & (solution_count - 1)) == 0) {
      std::cerr << "... solutions found: " << solution_count << '\n';
    }

    return false;
  };

  AlgorithmDLX dlx(std::move(lm));
  dlx.search(callback);
  std::cout << "solutions: " << solution_count << '\n';
}
