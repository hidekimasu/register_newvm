#!/usr/bin/perl
use strict;
use warnings;

require( './sub.pl' );
my $array_ref = method();

print join( ",", @{$array_ref}), "\n";
