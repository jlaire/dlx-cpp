Dancing Links
=============

This is an efficient C++ implementation of Algorithm DLX from Knuth's *Dancing
Links* paper for the exact cover problem.

Knuth's paper gives a very readable explanation of the problem, the algorithm,
and several applications. http://arxiv.org/pdf/cs/0011047v1.pdf

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

The executable `dlx` reads an exact cover problem from stdin and solves it. The
first line should contain one integer, the number of columns, and the following
lines contain the rows of the matrix.

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
primary columns. The number of secondary columns can be given on the first line
of input, right after the width of the matrix. It defaults to zero, i.e., a
regular exact cover problem.

A very trivial example:

    $ ./build/dlx -pv < data/generalized_example.txt
    0 1 1

    solutions: 1

Example: Sudoku
===============

Some interesting problems, such as Sudoku, are just exact cover in disguise.
What makes this really interesting is that DLX is often more efficient than
hand-written solutions!

[example/sudoku](example/sudoku) is based on `dlx` and can solve even
pathological 17-clue Sudokus in under a millisecond. There is a big
downloadable collection at
http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php, and you can feed it to
the executable directly. There are also a few puzzles in
[data/sudokus.txt](data/sudokus.txt).

    $ make examples
    $ ./build/sudoku < data/sudokus.txt
    846937152319625847752184963285713694463859271971246385127598436638471529594362718
    145327698839654127672918543496185372218473956753296481367542819984761235521839764
    693784512487512936125963874932651487568247391741398625319475268856129743274836159
    793684512486512937125973846932751684578246391641398725319465278857129463264837159
    961845327458723169237169584796358412524691873813274956182436795379582641645917238
    378564192921387456654291738489672315713458269265913847897135624146829573532746981
    681739245497562813523841769172954386865317924349628571916283457234175698758496132
    352718946914635872687492315763984521129567438845123769298341657471856293536279184


TODO
====

  - CMake, more unit tests, more examples.
  - Build `clx` as a library.
  - Make `NodeId` a template parameter to save space when possible.
  - Optimize `LinkedMatrix` construction; it uses temporary nodes that are not freed.
