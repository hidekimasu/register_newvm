#! /usr/bin/perl

use strict;
use warnings;

use LWP::UserAgent;
use JSON;
use Data::Dumper;


sub login {
  my ($host, $name, $pswd, $vdrk) = @_;

  my $ua = LWP::UserAgent->new;
  $ua->ssl_opts(verify_hostname => 0, SSL_verify_mode => 0x00);

  my $url = "https://".$host."/actifio/api/login";

  $url = $url."?name=".$name."&password=".$pswd."&vendorkey=".$vdrk;

  my $request = HTTP::Request->new(GET => $url);

  my $response = $ua->request($request);

  if( $response->{_rc} == 200) {
      my $items = JSON->new()->decode($response->{_content});

      foreach my $item ( $items ) {
          return $item->{sessionid};
      }
  }
}

sub lshost {
  $sessionid = shift @_;


}

&main();

sub main() {
  my $actifio = "192.168.99.174";
  my $name = "admin";
  my $pswd = "insight";
  my $vdrk = "actifio";

  my $sessionid = login( $actifio, $name, $pswd, $vdrk);
  print $sessionid;
  lshost( $sessionid );
}


