#include <iostream>
#include <unordered_map>
#include <vector>

struct box {
	int val; // size for columns, row index for cells
	box *l, *r, *u, *d, *col;
	box() {
		l = r = u = d = col = this;
	}
	void hide_lr() { l->r = r, r->l = l; }
	void show_lr() { l->r = r->l = this; }
	void hide_ud() { u->d = d, d->u = u; }
	void show_ud() { u->d = d->u = this; }
	void link_l(box *x) { x->r = this, x->l = l; l = l->r = x; }
	void link_u(box *x) { x->d = this, x->u = u; u = u->d = x; }
};

box *create_box_from_boolean_matrix(const std::vector<std::vector<int>>& matrix) {
	box* h = new box;
	if (matrix.empty()) {
		return h;
	}
	unsigned width = matrix[0].size();
	for (unsigned i = 0; i < width; ++i) {
		box *col = new box;
		col->val = 0;
		h->link_l(col);
	}
	for (unsigned i = 0; i < matrix.size(); ++i) {
		auto& matrix_row = matrix[i];
		if (matrix_row.size() != width) {
			return nullptr;
		}
		box *row = new box;
		box *col = h;
		for (int x : matrix_row) {
			col = col->r;
			if (x == 0) {
				continue;
			}
			box *cell = new box;
			cell->val = i;
			cell->col = col;
			col->link_u(cell);
			++col->val;
			row->link_l(cell);
		}
		row->hide_lr();
		delete row;
	}
	return h;
}

void dump(box *root) {
	std::cout << "dump" << std::endl;
	for (box *col = root->r; col != root; col = col->r) {
		for (box *cell = col->d; cell != col; cell = cell->d) {
			std::cout << " " << cell->val;
		}
		std::cout << std::endl;
	}
}

static void cover_column(box *col) {
	col->hide_lr();
	for (box *row = col->d; row != col; row = row->d) {
		for (box *cell = row->r; cell != row; cell = cell->r) {
			cell->hide_ud();
			--cell->col->val;
		}
	}
}

static void uncover_column(box *col) {
	for (box *row = col->u; row != col; row = row->u) {
		for (box *cell = row->l; cell != row; cell = cell->l) {
			cell->show_ud();
			++cell->col->val;
		}
	}
	col->show_lr();
}

void dlx(box *root, std::vector<int>& stack, std::vector<std::vector<int>>& solutions) {
	if (root->r == root) {
		bool first = true;
		for (int row : stack) {
			if (!first) {
				std::cout << " ";
			}
			first = false;
			std::cout << row;
		}
		std::cout << std::endl;
		solutions.push_back(stack);
		return;
	}
	box *col = root->r;
	int min_size = col->val;
	for (box *c = col->r; c != root; c = c->r) {
		if (c->val < min_size) {
			min_size = c->val;
			col = c;
		}
	}
	if (min_size < 1) {
		return;
	}
	cover_column(col);
	for (box *row = col->d; row != col; row = row->d) {
		stack.push_back(row->val);
		for (box *cell = row->r; cell != row; cell = cell->r) {
			cover_column(cell->col);
		}
		dlx(root, stack, solutions);
		for (box *cell = row->l; cell != row; cell = cell->l) {
			uncover_column(cell->col);
		}
		stack.pop_back();
	}
	uncover_column(col);
}

int solve(box *root) {
	std::vector<int> stack;
	std::vector<std::vector<int>> solutions;
	dlx(root, stack, solutions);
	return solutions.size();
}

int main() {
	int width = 0;
	std::cin >> width;
	std::vector<std::vector<int>> matrix;
	while (std::cin) {
		std::vector<int> row(width);
		for (int i = 0; i < width; ++i) {
			std::cin >> row[i];
		}
		matrix.emplace_back(row);
	}
	box *root = create_box_from_boolean_matrix(matrix);

	std::cout << solve(root) << std::endl;
}
