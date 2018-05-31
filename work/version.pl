#! /usr/bin/perl

use strict;
use warnings;

use LWP::UserAgent;
use Data::Dumper;

my $ua = LWP::UserAgent->new;


#$ua->ssl_opts(verify_hostname => 0);
#$ua->ssl_opts(SSL_verify_mode => 0x00);
$ua->ssl_opts(verify_hostname => 0, SSL_verify_mode => 0x00);

my $url = "https://192.168.99.171/actifio/api/version";

print $url."\n";

my $request = HTTP::Request->new(GET => $url);
my $response = $ua->request($request);

print Dumper $response;


