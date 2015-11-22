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
  bool opt_print_tree = false;

  for (int opt; (opt = getopt(argc, argv, "pvst")) != -1;) {
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
    case 't':
      opt_print_tree = true;
      break;
    default:
      std::cerr << "Unexpected option: '" << opt << "'\n";
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

  AlgorithmDLX dlx(std::move(lm));
  auto result = dlx.search({});
  for (const auto& row_indices : result.solutions) {
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
  }
  std::cout << "solutions: " << result.number_of_solutions << '\n';

  if (opt_print_tree) {
    std::cout << '\n';
    for (auto i = 0u; i < result.nodes_at_depth.size(); ++i) {
      std::cout << "Nodes at depth " << i << ": " << result.nodes_at_depth[i] << '\n';
    }
  }
}
