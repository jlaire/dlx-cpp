Sudoku
======

To be documented.

Examples:

```
./build/sudoku < data/sudokus.txt
./build/sudoku -s -f compact < data/sudokus.txt
./build/sudoku -v -f oneline < data/sudokus.txt

# sudoku17 can be downloaded form interwebs
./build/sudoku -s -l -f oneline < sudoku17 > sudoku17_solutions
```

TODO:
  - Use proper exception types and messages.
  - Generate sudokus.
  - Generate random jigsaw patterns.
  - A format that uses / and \ when appropriate?
