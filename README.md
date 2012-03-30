Minimal C++ implementation of Knuth's Dancing Links algorithm.

dlx reads from stdin and prints to stdout. First line of input should contain
two integers, the number of columns and the number of secondary columns. For
normal exact cover problems, the second number is 0. If it's positive, the
leftmost columns are the secondary ones.

The rest of input contains the matrix.

Output can be controlled by flags. By default, only the number of solutions is
printed. If -p is given, every solution is printed on its own line by giving
the rows' indices. With -v, the full rows are printed and solutions are
separated by two newlines.

    $ make
    g++ -std=c++0x -Wall -o dlx dlx.cpp
    $ cat knuth-example.in
    7 0
    0 0 1 0 1 1 0
    1 0 0 1 0 0 1
    0 1 1 0 0 1 0
    1 0 0 1 0 0 0
    0 1 0 0 0 0 1
    0 0 0 1 1 0 1
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

    $ cat knuth-example-sparse.in 
    7 0
    2 4 5
    0 3 6
    1 2 5
    0 3
    1 6
    3 4 6
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

TODO
====

  - Plain pointers and new are used, and I'm not even trying to clean up.

  - Examples.
