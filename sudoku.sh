#!/bin/bash

perl sudoku/sudoku.pl < $1 > sudoku.spec.$$ && perl wrap.pl 'in' sudoku.spec.$$ | ./dlx -ps | perl wrap.pl out sudoku.spec.$$ | perl sudoku/sudoku_show.pl $1 && rm sudoku.spec.$$
