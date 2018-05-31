#! /usr/bin/perl

use strict;
use warnings;

use LWP::UserAgent;
use JSON;
use Data::Dumper;


sub act_login {
  my ($actifio, $name, $pswd, $vdrk) = @_;

  my $ua = LWP::UserAgent->new;
  $ua->ssl_opts(verify_hostname => 0, SSL_verify_mode => 0x00);

  my $url = "https://".$actifio."/actifio/api/login";

  $url = $url."?name=".$name."&password=".$pswd."&vendorkey=".$vdrk;

  my $request = HTTP::Request->new(GET => $url);

  my $response = $ua->request($request);

  if( $response->{_rc} == 200) {
      my $items = JSON->new()->decode($response->{_content});

      foreach my $item ( $items ) {
#	  print Dumper ($item);
          return $item->{sessionid};
      }
  }
}

sub act_lshost {
  my ($actifio, $sessionid) = @_;

  my $ua = LWP::UserAgent->new;
  $ua->ssl_opts(verify_hostname => 0, SSL_verify_mode => 0x00);

  my $url = "https://".$actifio."/actifio/api/info/lshost";

  $url = $url."?sessionid=".$sessionid;

  my $request = HTTP::Request->new(GET => $url);
  my $response = $ua->request($request);

#print Dumper ($response);

  if( $response->{_rc} == 200) {
      my $items = JSON->new()->decode($response->{_content});

      my $lists=$items->{result};
      my $num=@$lists;
      my @hostnames;
      foreach my $item ( @$lists ) {
        print $item->{hostname}."\n";
        push @hostnames, $item->{hostname};
      }
      return @hostnames;
  }
}

&main();

sub main() {
  my $actifio = "192.168.99.171";
  my $name = "admin";
  my $pswd = "insight";
  my $vdrk = "actifio";
  my $vcent_name = "administrator\@vsphere.local";
  my $vcent_pass = "Insight1310!";
  my $vcent_svr  = "192.168.99.166";

  my $sessionid = act_login( $actifio, $name, $pswd, $vdrk);
  print $sessionid."\n";
  my @hostnames = act_lshost( $actifio, $sessionid );

foreach my $hostname (@hostnames) {
  print $hostname."\n";
}
  require ( './list_vms_fix.pl');
  
  vmware_init( $vcent_name, $vcent_pass, $vcent_svr);
  vmware_setup();
  vmware_run();

}


