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

looks like this in C++ ([AlgorithmDLX.cpp](src/AlgorithmDLX.cpp#L27)):

```
cover_column(c);
for (auto r = D(c); r != c; r = D(r)) {
  O.push_back(row(r));
  for (auto j = R(r); j != r; j = R(j))
    cover_column(j);
  search();
  for (auto j = L(r); j != r; j = L(j))
    uncover_column(j);
  O.pop_back();
}
uncover_column(c);
```

Usage
=====

The implementation consists of two classes, `AlgorithmDLX` and `LinkedMatrix`,
that can be used as a library.

[example/dlx](example/dlx) is a command-line program that reads an exact cover
problem from stdin and solves it.

The first line should contain an integer, the number of columns, and the
following lines contain the rows of the matrix.

Output can be controlled by flags. By default, only the number of solutions is
printed. If `-p` is given, every solution is printed on its own line by giving
the indices of the selected rows. With `-v`, the full rows are printed and
solutions are separated by two newlines.

    $ make
    $ ./build/dlx < data/knuth_example.txt
    solutions: 1
    $ ./build/dlx -p < data/knuth_example.txt
    3 0 4
    solutions: 1
    $ ./build/dlx -pv < data/knuth_example.txt
    1 0 0 1 0 0 0
    0 0 1 0 1 1 0
    0 1 0 0 0 0 1

    solutions: 1

With `-s`, input can also be given as a sparse matrix. The output of `-v` will
also be sparse.

    $ ./build/dlx -pvs < data/knuth_example_sparse.txt 
    0 3
    2 4 5
    1 6

    solutions: 1

Generalized exact cover
=======================

`dlx` can also solve generalized exact cover problems. The columns of the
matrix should be sorted so that all secondary columns are on the left, before
primary columns. [example/dlx](example/dlx) supports this, and the number of
secondary columns can be given on the first line of input, right after the
width of the matrix. It defaults to zero, i.e., a regular exact cover problem.

A very trivial example:

    $ ./build/dlx -pv < data/generalized_example.txt
    0 1 1

    solutions: 1

Example: Langford pairings
==========================

See [Wikipedia](https://en.wikipedia.org/wiki/Langford_pairing).

    $ make examples
    $ ./build/langford -v <<< '1 2 3 4 5'
    Number of unique Langford pairings for n = 1: 0
    Number of unique Langford pairings for n = 2: 0
    Number of unique Langford pairings for n = 3: 1
    3 1 2 1 3 2
    Number of unique Langford pairings for n = 4: 1
    4 1 3 1 2 4 3 2
    Number of unique Langford pairings for n = 5: 0

Example: Sudoku
===============

This program can solve various types of Sudokus given in plaintext. See
[example/sudoku/README.md](example/sudoku/README.md) for details.

<big><pre>
$ make examples
$ ./build/sudoku < data/sudoku.txt
[output](https://gist.github.com/jlaire/9195d1e5640440de5160)
</pre></big>

TODO
====

  - AlgorithmDLX: Modify `search()` to call the callback on every level, so
    that it can be used to construct the shape of the search tree.
  - CMake, more unit tests, more examples.
