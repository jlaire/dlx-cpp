Sudoku
======

To be documented.

Examples:

```
./build/sudoku -i < data/sudokus.txt
./build/sudoku -i -f compact < data/sudokus.txt
./build/sudoku -i -f preserve < data/sudokus.txt
./build/sudoku -i -f oneline < data/sudokus.txt

# sudoku17 can be downloaded form interwebs
./build/sudoku -l -f oneline < sudoku17 > sudoku17_solutions
```

TODO:
  - Use proper exception types and messages.
  - Generate sudokus.
  - Generate random jigsaw patterns.
  - Option to print initial and solved grids side-by-side.
  - A format that uses / and \ when appropriate?
