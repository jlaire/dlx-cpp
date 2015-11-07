#!/usr/bin/perl
use v5.12;
use warnings FATAL => 'all';
use strict;

my $usage = 'Usage: perl wrap.pl [in|out] spec.in';

my $action = shift // die $usage;
die $usage if $action !~ /^(?:in|out)$/;
my $spec = shift // '/dev/stdin';
open IN, '<', $spec or die $!;

my @cols = sort grep /\S/, split /\s+/, <IN>;
my @rows = map [grep /\S/, split /\s+/], <IN>;
my @row_names = map { shift @$_ } @rows;
my %col_names = map { $cols[$_] => $_ } 0 .. $#cols;

if ($action eq 'in') {
  say scalar(@cols);
  for (@rows) {
    say join " ", map $col_names{$_}, @$_;
  }
}
elsif ($action eq 'out') {
  while (<STDIN>) {
    if (/^solutions: \d+$/) {
      print;
    }
    else {
      say join " ", map { $row_names[$_] // $_ } split /\s+/;
    }
  }
}
else {
  die $usage;
}
