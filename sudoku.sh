#!/bin/bash

perl sudoku.pl < $1 > sudoku.spec.$$ && perl wrap.pl 'in' sudoku.spec.$$ | ./dlx -p | perl wrap.pl out sudoku.spec.$$ | perl sudoku_show.pl $1 && rm sudoku.spec.$$
