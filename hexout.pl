#!/usr/bin/perl
use warnings FATAL => 'all';
use strict;

my $mode = shift // 'gen';
my $full = $mode eq 'genfull';

my @points = map "P$_", grep /\S/, split /\s+/, `cat points.txt`;
my @grooves = map [grep /\S/, split /\s+/], split /\n\n/, `cat grooves.txt`;

my @names = map [], 0 .. $#points;
for my $groove (@grooves) {
	push @{$names[$_]}, $groove->[$_] for 0 .. @$groove - 1;
}

my %p = map { $_ => [] } @points;
for my $p (0 .. $#names) {
	$p{$_} = $points[$p] for @{$names[$p]};
}

my %groove_size = map { $_ => 1 } 'a' .. 'w';
for (map @$_, @grooves) {
	/(\w)(\d)/ or next;
	$groove_size{$1} = $2 if $2 > $groove_size{$1};
}
my %rows;
for my $c ('a' .. 'w') {
	my $s = $groove_size{$c};
	for my $l (($full ? 1 : 2) .. 3) {
		for (1 .. $s-$l+1) {
			my %ps = map { $p{$c . $_} => 1 } $_ .. $_+$l-1;
			my $name = $c . join "", $_ .. $_+$l-1;
			$rows{$name} = join " ", map { $ps{$_} // 0 } @points;
		}
	}
}

if ($mode eq 'gen') {
	print scalar(@points) . " 0 \n";
	print "$rows{$_}\n" for sort keys %rows;
}
elsif ($mode eq 'decode') {
	my @row_names = sort keys %rows;
	while (<STDIN>) {
		print join(" ", map $row_names[$_] // $_, grep /\S/, split /\s+/) . "\n";
	}
}
else {
	print "unknown mode: $mode\n";
	exit 1;
}
