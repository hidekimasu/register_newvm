#!/usr/bin/perl
use strict;
use warnings;

#
########################################################
# Copyright (c) VMware, Inc. 2017.  All Rights Reserved.
# SPDX-License-Identifier: MIT
########################################################
#
# DISCLAIMER. THIS PROGRAM IS PROVIDED TO YOU "AS IS" WITHOUT
# WARRANTIES OR CONDITIONS # OF ANY KIND, WHETHER ORAL OR WRITTEN,
# EXPRESS OR IMPLIED. THE AUTHOR SPECIFICALLY # DISCLAIMS ANY IMPLIED
# WARRANTIES OR CONDITIONS OF MERCHANTABILITY, SATISFACTORY # QUALITY,
# NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
#

my $__author__ = 'VMware, Inc.';
my $__copyright__ = 'Copyright 2017 VMware, Inc. All rights reserved.';
my $__vcenter_version__ = '6.5+';

use lib '/home/masu/vsphere-automation-sdk-perl/lib/runtime','/home/masu/vsphere-automation-sdk-perl/samples','/home/masu/vsphere-automation-sdk-perl/lib/sdk';

#
# Perl Core Modules
#
use strict;
use Getopt::Long;

use Data::Dumper;

#
# VMware runtime library
#
use Com::Vmware::Vapi::Util::Logger
  qw(log_info log_dumper log_framework log_warning set_verbosity);

#
# Sample helper module
#
use Common::SampleBase;

#
# Generated SDK's
#
use Com::Vmware::Cis::Session;
use Com::Vmware::Vcenter::VM;
use Com::Vmware::Vcenter::Vm::Power;

# Set the logger level
set_verbosity( 'level' => 0 );

# Initialize the global variable
my ( %params, $sampleBase, $stubFactory, $stubConfig, $vm_service ) = ();

# Declare the mandatory parameter list
my @required_options = ( 'username', 'password', 'server' );

sub vmware_init {
   my( $username, $password, $server)=@_;

   %params = (
	'username' => $username,
	'password' => $password,
	'server'   => $server,
   );

   $sampleBase  = new Common::SampleBase( 'params' => \%params );
   $stubConfig  = $sampleBase->{'stub_config'};
   $stubFactory = $sampleBase->{'stub_factory'};
}

sub vmware_setup {

   # Get the VM Service
   $vm_service = $stubFactory->create_stub(
      'service_name' => 'Com::Vmware::Vcenter::VM',
      'stub_config'  => $stubConfig
   );
}

sub vmware_run {
    my @vm_hosts;
    my $vm_list = $vm_service->list();
    foreach my $vm (@$vm_list) {
      push @vm_hosts, $vm->get_name();
        print $vm->get_name()."\n";
#print Dumper($vm);
    }
    return \@vm_hosts;
}

#
# Demonstrates getting list of VMs present in vCenter.
#
# Sample Prerequisites:
# vCenter/ESX
#

# Call main
#&main();

#sub main() {
#   vmware_init( 'root', 'insight', '192.168.99.85');
#   vmware_setup();
#   vmware_run();
#   log_info( MSG => "#### Done!" );
#}
1;

# END


