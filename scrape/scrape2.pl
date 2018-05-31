#! /usr/bin/perl
use strict;
use warnings;

use HTTP::Tiny;

my $http = HTTP::Tiny->new( verify_SSL => 0, );

for my $i (1..254) {
  my $url = "https://192.168.99.".$i."/ui/#/login";
  my $res = $http->get( $url );

  my $body = $res->{content};

#print $body;
  my @lines = split /\n/, $body;

  for my $line ( @lines ) {
    if ( $line =~ /ng-app=\"esxUiApp\"/) {
       print $url."\n";
       print $line;
    }

  }
}

