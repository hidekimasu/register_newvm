#! /usr/bin/perl

use strict;
use warnings;

use LWP::UserAgent;
use JSON;
use Data::Dumper;

my $ua = LWP::UserAgent->new;

#$ua->agent("MyApp/0.1");

$ua->ssl_opts(verify_hostname => 0, SSL_verify_mode => 0x00);

my $url = "https://192.168.99.10/actifio/api/login";
my $name = "admin";
my $pswd = "insight";
my $vdrk = "actifio";

$url = $url."?name=".$name."&password=".$pswd."&vendorkey=".$vdrk;

print $url."\n";

#my $data = {
#  name => $name,
#  password => $pswd,
#  vendorkey => $vdrk,
#};

my $request = HTTP::Request->new(GET => $url);
#$request->content($data);

print Dumper $request;

my $response = $ua->request($request);

print Dumper $response;

if( $response->{_rc} == 200) {
    my $items = JSON->new()->decode($response->{_content});

    print Dumper $items;

    foreach my $item ( $items ) {
        printf $item->{sessionid};
    }
}


#my @headers = ("Authorization: Token xert345llkd736gdy3");
#foreach my $ml(@headers) {
#  $auth = $ml;
#}
#
#
#my $data = {
#  name => 'john',
#  country=> 'ca',
#  email=> 'john@email.com',
#};
#
#my $req = HTTP::Request->new(POST => '$url', 
#               'Content-type' => 'application/json',
#               'Authorization' => "$auth"
#);
#$req->content_type('application/json');
#$req->content($data);
#
#my $res = $ua->request($req);
#
#if ($res->is_success) {
#  print $res->content;
#} else {
#  print $res->status_line, "n";
#}


