#!/usr/bin/perl

use strict;
use warnings;
use JSON;

my $json_data = '[{"name":"foo","id":"100"},{"name":"bar","id":"110"},{"name":"baz","id":"120"}]';
my $items = JSON->new()->decode($json_data);

my $num=@$items;
print $num."\n";
foreach my $item (@$items) {
    printf "%s,%s\n", $item->{name}, $item->{id};
}

