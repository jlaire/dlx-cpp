#!/usr/bin/perl
use v5.20;

my $width = shift() // 5;
my $height = shift() // 10;

say $width;
for (1 .. $height) {
  say join " ", map { int rand 2 } 1 .. $width;
}
