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

sub act_vmdiscovery {
  my ($actifio, $sessionid, $vcent_hname, $vm_hosts) = @_;

  my $ua = LWP::UserAgent->new;
  $ua->ssl_opts(verify_hostname => 0, SSL_verify_mode => 0x00);
  my $url = "https://".$actifio."/actifio/api/task/vmdiscovery?sessionid=".$sessionid."&addvms&host=".$vcent_hname."&cluster=192.168.99.85&vms=".$vm_hosts;

  my $request = HTTP::Request->new(POST => $url);
  my $response = $ua->request($request);
  print Dumper $response;
}


&main();

sub main() {
  my $actifio = "192.168.99.171";
  my $name = "admin";
  my $pswd = "insight";
  my $vdrk = "actifio";
  my $vcent_uname = "administrator\@vsphere.local";
  my $vcent_pass  = "Insight1310!";
  my $vcent_svr   = "192.168.99.166";
  my $vcent_hname = "vCenter6.5";

  my $sessionid = act_login( $actifio, $name, $pswd, $vdrk);
  print $sessionid."\n";
  my @hostnames = act_lshost( $actifio, $sessionid );

foreach my $hostname (@hostnames) {
  print $hostname."\n";
}
  require ( './vmware_vms_list.pl');
  
  vmware_init( $vcent_uname, $vcent_pass, $vcent_svr);
  vmware_setup();
  my $vm_hosts_ref = vmware_run();

print "---- vm_hosts ----\n";
my $i=0;
foreach my $vm_host ( @{$vm_hosts_ref}) {
  print "$i $vm_host\n";
  $i++;
}
print "-----end---\n";

  foreach my $hostname ( @hostnames ) {
    $i=0;
    foreach my $vm_host ( @{$vm_hosts_ref} ) {
      if ( $hostname eq $vm_host ) {
        splice( @{$vm_hosts_ref}, $i, 1);
        $i=0;
      } else {
        $i++;
      }
    }
  }
$i=0;
foreach my $vm_host ( @{$vm_hosts_ref}) {
  print "$i $vm_host\n";
  $i++;
}
#print @{$vm_hosts_ref}[1];   # rhel5-ora11-997-mount
  my $vm_hosts = join (':', @{$vm_hosts_ref});

  #テスト用にVMは２つ
  $vm_hosts = @{$vm_hosts_ref}[1].":".@{$vm_hosts_ref}[5];
print $vm_hosts."\n";
  act_vmdiscovery( $actifio, $sessionid, $vcent_hname, $vm_hosts);
}


