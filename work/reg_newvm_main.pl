#! /usr/bin/perl

use strict;
use warnings;

use LWP::UserAgent;
use JSON;
use Data::Dumper;

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

  require ( './reg_newvm_act.pm' );
  require ( './reg_newvm_vmware.pm');

  my $sessionid = act_login( $actifio, $name, $pswd, $vdrk);
  print $sessionid."\n";
  my @hostnames = act_lshost( $actifio, $sessionid );

foreach my $hostname (@hostnames) {
  print $hostname."\n";
}
  
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


