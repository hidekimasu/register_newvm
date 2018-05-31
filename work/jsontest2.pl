#!/usr/bin/perl

use strict;
use warnings;
use JSON;

sub listnames {
    my $json_data = '[{"name":"foo","id":"100"},{"name":"bar","id":"110"},{"name":"baz","id":"120"}]';
    my $items = JSON->new()->decode($json_data);

    my $num=@$items;
    print $num."\n";
    my @names;
    foreach my $item (@$items) {
        printf "%s,%s\n", $item->{name}, $item->{id};
        push @names, $item->{name};
    }
    return @names;
}

&main();

sub main() {
    my @names=listnames();
    print @names;
    print "\n";
}

