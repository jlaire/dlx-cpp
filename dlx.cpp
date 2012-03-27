#include <iostream>
#include <unordered_map>
#include <vector>

struct box {
	int x, y, size;
	box *l, *r, *u, *d;
	box() {
		l = r = u = d = this;
	}
	void hide_lr() { l->r = r, r->l = l; }
	void show_lr() { l->r = r->l = this; }
	void hide_ud() { u->d = d, d->u = u; }
	void show_ud() { u->d = d->u = this; }
	void link_l(box *b) { b->r = this, b->l = l; l = l->r = b; }
	void link_u(box *b) { b->d = this, b->u = u; u = u->d = b; }
};

struct linked_matrix {
	box *root;
	std::vector<box *> cols;
};

linked_matrix *linked_matrix_from_boolean_rows(const std::vector<std::vector<int>>& rows) {
	linked_matrix *lm = new linked_matrix;
	lm->root = new box;
	if (rows.empty()) {
		return lm;
	}
	unsigned width = rows[0].size();
	lm->cols.resize(width);
	for (unsigned i = 0; i < width; ++i) {
		box *col = new box;
		col->size = 0;
		col->x = i;
		lm->cols[i] = col;
		lm->root->link_l(col);
	}
	for (unsigned i = 0; i < rows.size(); ++i) {
		auto& matrix_row = rows[i];
		if (matrix_row.size() != width) {
			return nullptr;
		}
		box *row = new box;
		box *col = lm->root;
		for (unsigned j = 0; j < width; ++j) {
			col = col->r;
			if (matrix_row[j] == 0) {
				continue;
			}
			box *cell = new box;
			cell->x = j;
			cell->y = i;
			col->link_u(cell);
			++col->size;
			row->link_l(cell);
		}
		row->hide_lr();
		delete row;
	}
	return lm;
}

void dump(box *root) {
	std::cout << "dump" << std::endl;
	for (box *col = root->r; col != root; col = col->r) {
		for (box *cell = col->d; cell != col; cell = cell->d) {
			std::cout << " " << cell->y;
		}
		std::cout << " (" << col->size << ")";
		std::cout << std::endl;
	}
}

static void cover_column(linked_matrix *lm, box *col) {
	col->hide_lr();
	for (box *row = col->d; row != col; row = row->d) {
		for (box *cell = row->r; cell != row; cell = cell->r) {
			cell->hide_ud();
			--lm->cols[cell->x]->size;
		}
	}
}

static void uncover_column(linked_matrix *lm, box *col) {
	for (box *row = col->u; row != col; row = row->u) {
		for (box *cell = row->l; cell != row; cell = cell->l) {
			cell->show_ud();
			++lm->cols[cell->x]->size;
		}
	}
	col->show_lr();
}

void dlx(linked_matrix *lm, std::vector<int>& stack, std::vector<std::vector<int>>& solutions) {
	if (lm->root->r == lm->root) {
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
	box *col = lm->root->r;
	int min_size = col->size;
	for (box *c = col->r; c != lm->root; c = c->r) {
		if (c->size < min_size) {
			min_size = c->size;
			col = c;
		}
	}
	if (min_size < 1) {
		return;
	}
	cover_column(lm, col);
	for (box *row = col->d; row != col; row = row->d) {
		stack.push_back(row->y);
		for (box *cell = row->r; cell != row; cell = cell->r) {
			cover_column(lm, lm->cols[cell->x]);
		}
		dlx(lm, stack, solutions);
		for (box *cell = row->l; cell != row; cell = cell->l) {
			uncover_column(lm, lm->cols[cell->x]);
		}
		stack.pop_back();
	}
	uncover_column(lm, col);
}

int solve(linked_matrix *lm) {
	std::vector<int> stack;
	std::vector<std::vector<int>> solutions;
	dlx(lm, stack, solutions);
	return solutions.size();
}

int main() {
	int width = 0;
	std::cin >> width;
	std::vector<std::vector<int>> rows;
	while (std::cin) {
		std::vector<int> row(width);
		for (int i = 0; i < width; ++i) {
			std::cin >> row[i];
		}
		rows.emplace_back(row);
	}
	linked_matrix *lm = linked_matrix_from_boolean_rows(rows);

	std::cout << solve(lm) << std::endl;
}
