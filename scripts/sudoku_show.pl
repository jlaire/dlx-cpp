#!/usr/bin/perl
use v5.12;
use strict;
use warnings FATAL => 'all';

my $file = shift // die;
open GRID, '<', $file or die $!;

my $grid = join "", <GRID>;
$grid =~ s/\D//g;
die "bad input" if length $grid != 81;
my @grid = map [split //], $grid =~ /(\d{9})/g;

while (<STDIN>) {
	next if /^solutions/;
	for (split /\s+/) {
		/(\d)(\d)=(\d)/ or next;
		$grid[$2][$1] = $3;
	}
	say @$_ for @grid;
}
