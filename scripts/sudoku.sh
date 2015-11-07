#!/bin/bash

perl sudoku/sudoku.pl < $1 > data/sudoku.spec.$$
perl scripts/wrap.pl 'in' data/sudoku.spec.$$ \
  | ./dlx -ps \
  | perl scripts/wrap.pl out data/sudoku.spec.$$ \
  | perl sudoku/sudoku_show.pl $1
rm data/sudoku.spec.$$
