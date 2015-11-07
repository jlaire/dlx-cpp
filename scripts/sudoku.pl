#!/usr/bin/perl
use v5.12;
use strict;
use warnings FATAL => 'all';

my $input = join "", <STDIN>;
$input =~ s/\D//g;
die "bad input" if length $input != 81;
my @grid = map [split //], $input =~ /(\d{9})/g;

sub at($$) { my ($x, $y) = @_; "P$x$y"; }
sub row($$) { my ($y, $dig) = @_; "R$y$dig" }
sub col($$) { my ($x, $dig) = @_; "C$x$dig" }
sub sq2($$) { my ($s, $dig) = @_; "S$s$dig" }
sub sqr($$$) { my ($x, $y, $dig) = @_; sq2(int($y/3)*3 + int($x/3), $dig) }

my %cols = map {
    my $x = $_ % 9;
	my $y = int($_ / 9);
	my $d = $y + 1;
	(at($x, $y) => 1, row($x, $d) => 1, col($x, $d) => 1, sq2($x, $d) => 1)
} 0 .. 80;
for my $y (0 .. 8) {
	for my $x (0 .. 8) {
		my $dig = $grid[$y][$x];
		if ($dig != 0) {
			sub del {
				my ($href, $key, $msg) = @_;
				die $msg unless exists $href->{$key};
				delete $href->{$key};
			}
			del \%cols, at($x, $y), "this doesn't happen";
			del \%cols, row($y, $dig), "more than one $dig in row " . ($y + 1);
			del \%cols, col($x, $dig), "more than one $dig in column " . ($x + 1);
			del \%cols, sqr($x, $y, $dig), "more than one $dig in a 3x3 box";
		}
	}
}

my %rows;
for my $y (0 .. 8) {
	for my $x (0 .. 8) {
		next if $grid[$y][$x] != 0;
		for my $dig (1 .. 9) {
			my @cols = (at($x, $y), row($y, $dig), col($x, $dig), sqr($x, $y, $dig));
			next if grep { not exists $cols{$_} } @cols;
			$rows{$x . $y . '=' . $dig} = [@cols];
		}
	}
}

say join " ", sort keys %cols;
say join " ", $_, sort @{$rows{$_}} for keys %rows;
