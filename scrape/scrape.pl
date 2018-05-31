#! /usr/bin/perl
use strict;
use warnings;

use HTTP::Tiny;

my $http = HTTP::Tiny->new();

for my $i (100..254) {
  my $url = "http://192.168.99.".$i;
  my $res = $http->get( $url );

  my $body = $res->{content};

  my @lines = split /\n/, $body;

  for my $line ( @lines ) {
    if ( $line =~ /name=\"description/ ) {
       print $url."\n";
       print $line;
    }

  }
}

#print $body;
