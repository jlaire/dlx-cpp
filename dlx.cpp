#include "linked_matrix.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>
#include <unistd.h>

uint64_t solve(linked_matrix *lm);
uint64_t dlx(linked_matrix *lm, std::vector<int>& stack);
void print_vector(const std::vector<int>& xs);
void print_solution(const std::vector<int>& rows);

bool opt_print_solutions;
bool opt_verbose;
bool opt_sparse;
bool opt_running_count;
std::vector<std::vector<int>> input_rows;

int main(int argc, char *argv[]) {
	for (;;) {
		switch (getopt(argc, argv, "pvsr")) {
			case -1:
				goto getopt_done;
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
		}
	}
	getopt_done:

	unsigned width = 0;
	unsigned secondary_columns = 0;
	{
		std::string line;
		std::getline(std::cin, line);
		std::istringstream ss(line);
		ss >> width >> secondary_columns;
	}
	while (std::cin) {
		std::string line;
		std::getline(std::cin, line);
		std::istringstream ss(line);

		std::vector<int> row;
		if (opt_sparse) {
			unsigned x;
			while (ss >> x) {
				row.push_back(x);
			}
			std::sort(row.begin(), row.end());
		}
		else {
			row.resize(width);
			for (unsigned i = 0; i < width; ++i) {
				ss >> row[i];
			}
		}
		if (!row.empty()) {
			input_rows.emplace_back(row);
		}
	}
	linked_matrix *lm;
	if (opt_sparse) {
		lm = linked_matrix::from_sparse_matrix(input_rows, secondary_columns);
	}
	else {
		lm = linked_matrix::from_boolean_rows(input_rows, secondary_columns);
	}

	std::cout << "solutions: " << solve(lm) << std::endl;
}

uint64_t solve(linked_matrix *lm) {
	std::vector<int> stack;
	return dlx(lm, stack);
}

uint64_t dlx(linked_matrix *lm, std::vector<int>& stack) {
	static uint64_t counter;
	if (lm->root->r == lm->root) {
		if (opt_print_solutions) {
			print_solution(stack);
		}
		if (opt_running_count) {
			++counter;
			if ((counter & (counter - 1)) == 0) {
				std::cerr << "... solutions found: " << counter << std::endl;
			}
		}
		return 1;
	}
	box *col = lm->root->r;
	int min_size = col->size;
	for (box *c = col->r; c != lm->root; c = c->r) {
		if (c->size < min_size) {
			min_size = c->size;
			col = c;
		}
	}
	if (min_size < 1) {
		return 0;
	}
	uint64_t solutions = 0;
	lm->cover_column(col);
	for (box *row = col->d; row != col; row = row->d) {
		stack.push_back(row->y);
		for (box *cell = row->r; cell != row; cell = cell->r) {
			lm->cover_column(lm->cols[cell->x]);
		}
		solutions += dlx(lm, stack);
		for (box *cell = row->l; cell != row; cell = cell->l) {
			lm->uncover_column(lm->cols[cell->x]);
		}
		stack.pop_back();
	}
	lm->uncover_column(col);
	return solutions;
}

void print_vector(const std::vector<int>& xs) {
	bool first = true;
	for (int x : xs) {
		if (!first) {
			std::cout << " ";
		}
		first = false;
		std::cout << x;
	}
	std::cout << std::endl;
}

void print_solution(const std::vector<int>& row_indices) {
	if (opt_verbose) {
		for (int i : row_indices) {
			print_vector(input_rows[i]);
		}
		std::cout << std::endl;
	}
	else {
		print_vector(row_indices);
	}
}
