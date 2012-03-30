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
    $ cat knuth-example.in
    7 0
    0 0 1 0 1 1 0
    1 0 0 1 0 0 1
    0 1 1 0 0 1 0
    1 0 0 1 0 0 0
    0 1 0 0 0 0 1
    0 0 0 1 1 0 1
    $ ./dlx < knuth-example.in
    1
    $ ./dlx -p < knuth-example.in
    3 0 4
    1
    $ ./dlx -pv < knuth-example.in
    1 0 0 1 0 0 0
    0 0 1 0 1 1 0
    0 1 0 0 0 0 1

    1

TODO
====

  - Plain pointers and new are used, and I'm not even trying to clean up.

  - Examples.
