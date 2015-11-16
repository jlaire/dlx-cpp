Dancing Links
=============

This is an efficient C++ implementation of Algorithm DLX from Knuth's *Dancing
Links* paper for the exact cover problem.

Knuth's paper gives a very readable explanation of the problem, the algorithm,
and several applications. I think it's still the best place to start.
http://arxiv.org/pdf/cs/0011047v1.pdf

To make the code easy to follow, it matches the paper quite closely (to a
fault).

The core loop of the algorithm (page 5):

```
Cover column c.
For each r ← D[c], D[D[c]], ..., while r ≠ c,
  set O[k] ← r;
  for each j ← R[r], R[R[r]], ..., while j ≠ r,
    cover column j;
  search(k + 1);
  set r ← O[k] and c ← C[r];
  for each j ← L[r], L[L[r]], ..., while j ≠ r,
    uncover column j.
Uncover column c.
```

... looks like this in C++ ([AlgorithmDLX.cpp](src/AlgorithmDLX.cpp)):

```
cover_column(c);
for (auto r = D(c); r != c; r = D(r)) {
  O.push_back(Y(r));
  for (auto j = R(r); j != r; j = R(j))
    cover_column(j);
  search();
  for (auto j = L(r); j != r; j = L(j))
    uncover_column(j);
  O.pop_back();
}
uncover_column(c);
```

Implementation
==============

The implementation consists of two classes, `AlgorithmDLX` and `LinkedMatrix`,
that can be used as a library. There is no documentation at the moment, so take
a look at the examples.

`dlx` can also solve generalized exact cover problems (see Knuth's paper). The
columns of the matrix should be sorted so that all secondary columns are on the
left, before primary columns. N-queens is a good example of this.

Example: dlx
============

[example/dlx](example/dlx) is a simple command-line program that reads an exact
cover problem from stdin and solves it.

The first line should contain the number of columns, and the following lines
contain the rows of the matrix.

Output can be controlled by flags. By default, only the number of solutions is
printed. If `-p` is given, every solution is printed on its own line by giving
the indices of the selected rows. With `-v`, the full rows are printed.

<big><pre>
$ make
$ ./build/dlx -pv < [data/knuth\_example.txt](data/knuth_example.txt)
1 0 0 1 0 0 0
0 0 1 0 1 1 0
0 1 0 0 0 0 1

solutions: 1
</pre></big>

With `-s`, input can be given as a sparse matrix.

<big><pre>
$ ./build/dlx -ps < [data/knuth\_example\_sparse.txt](data/knuth_example_sparse.txt)
3 0 4
solutions: 1
</pre></big>

To solve a generalized exact cover problem, put the number of secondary columns
on the first line, after the number of all columns. The default value is zero,
in other words, a regular exact cover problem.

<big><pre>
$ ./build/dlx -pv < [data/generalized\_example.txt](data/generalized_example.txt)
0 1 1

solutions: 1
</pre></big>

Example: Sudoku
===============

This program can solve various types of Sudokus given in plaintext. See
[example/sudoku/README.md](example/sudoku/README.md) for details.

<big><pre>
$ make examples
$ ./build/sudoku < [data/sudoku.txt](data/sudoku.txt)
[output](https://gist.github.com/jlaire/9195d1e5640440de5160)
</pre></big>

Example: N-queens
=================

Place N queens on an NxN chessboard. This is a good example of a generalized
exact cover problem; each diagonal must contain *at most* one queen, but zero
is ok.

```
$ make examples
$ ./build/nqueens 8 12
Solutions for n=8: 92
Solutions for n=12: 14200
```

```
$ ./build/nqueens -v 1 2 3 4
Solutions for n=1: 1
Q

Solutions for n=2: 0
Solutions for n=3: 0
Solutions for n=4: 2
..Q.
Q...
...Q
.Q..

.Q..
...Q
Q...
..Q.
```

Example: Langford pairings
==========================

See [Wikipedia](https://en.wikipedia.org/wiki/Langford_pairing).

    $ make examples
    $ ./build/langford -v 1 2 3 4 5
    Solutions for n = 1: 0
    Solutions for n = 2: 0
    Solutions for n = 3: 1
    3 1 2 1 3 2
    Solutions for n = 4: 1
    4 1 3 1 2 4 3 2
    Solutions for n = 5: 0

TODO
====

  - AlgorithmDLX: Modify `search()` to call the callback on every level, so
    that it can be used to construct the shape of the search tree.
  - CMake, more unit tests, more examples.
