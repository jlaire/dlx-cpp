Minimal C++ implementation of Knuth's Dancing Links algorithm.

`dlx` reads from stdin and prints to stdout. The first line of input should
contain one integer, the number of columns.

The rest of the input contains the matrix.

Output can be controlled by flags. By default, only the number of solutions is
printed. If -p is given, every solution is printed on its own line by giving
the indices of the selected rows. With -v, the full rows are printed and
solutions are separated by two newlines.

    $ make
    g++ -std=c++0x -Wall -o dlx dlx.cpp
    $ ./dlx < knuth-example.in
    solutions: 1
    $ ./dlx -p < knuth-example.in
    3 0 4
    solutions: 1
    $ ./dlx -pv < knuth-example.in
    1 0 0 1 0 0 0
    0 0 1 0 1 1 0
    0 1 0 0 0 0 1

    solutions: 1

Input can also be given as a sparse matrix.

    $ ./dlx -s < knuth-example-sparse.in
    solutions: 1
    $ ./dlx -ps < knuth-example-sparse.in 
    3 0 4
    solutions: 1
    $ ./dlx -pvs < knuth-example-sparse.in 
    0 3
    2 4 5
    1 6

    solutions: 1

Generalized exact cover
=======================

`dlx` can also solve generalized exact cover problems. The columns of the
matrix should be sorted so that all secondary columns are on the left, before
primary columns. The number of secondary columns can be given on the first line
of input, after the number of columns. It defaults to zero, i.e., a regular
exact cover problem.

Sudoku example
==============

Some interesting problems are just exact cover in disguise. Sudoku for example.
You can solve Sudokus using `dlx` with a happy little wrapper script:

    $ ./sudoku.sh sudoku/in.1
    846937152
    319625847
    752184963
    285713694
    463859271
    971246385
    127598436
    638471529
    594362718

TODO
====

  - C++ Sudoku solver and more examples.
