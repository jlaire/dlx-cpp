Sudoku
======

If something is worth doing, it's worth doing well.

The [solver](SudokuSolver.cpp) itself is quite simple. There are four types of
columns, n\*n of each type. (For standard Sudoku, n = 9.)

  - Cell (*x*, *y*) needs to contain a digit.
  - Column *x* needs to contain digit *d*.
  - Row *y* needs to contain digit *d*.
  - Region *i* needs to contain digit *d*.

The first one can actually be either a secondary or primary column; if all
other conditions are met, every cell will naturally contain *at most one*
digit.

For the normal 9x9 sudoku, regions are 3x3 squares. But generalizing this so
that the regions are allowed to form an arbitrary partition of the grid does
not affect anything.

Each row of the matrix hits exactly 4 columns, one of each type.

Usage
=====

To be documented.

Examples
========

    ./build/sudoku < data/sudokus.txt
    ./build/sudoku -s -f compact < data/sudokus.txt
    ./build/sudoku -v -f oneline < data/sudokus.txt

    # sudoku17 can be downloaded form interwebs
    ./build/sudoku -s -l -f oneline < sudoku17 > sudoku17_solutions

TODO
====

  - Use proper exception types and messages.
  - Generate sudokus.
  - Generate random jigsaw patterns.
  - A format that uses / and \ when appropriate?
