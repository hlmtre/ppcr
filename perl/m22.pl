#!/usr/bin/perl
use warnings;
use strict;

my $arg = $ARGV[0];
my $numargs = $#ARGV + 1;
my $ifconfig = "/sbin/ifconfig";
my $iface = "wlan0";
my @lines = qx|$ifconfig $iface| or die ("Can't get info from ifconfig");
my $bool = 0;
foreach (@lines)
{
	if (/inet addr:192.168.1.104/)
	{ 
		$bool = 1;
	}
}
#print $arg;
#print $numargs;
if ($numargs < 1 || $arg eq "s")
{
	if ($bool == 1)
	{
		print "Connecting to minus22 locally...\n";
		exec ("ssh -p 117 hlmtre\@minus22");
	}
	else
	{
		print "Connecting to minus22 internetishly...\n";
		exec ("ssh -p 117 hlmtre\@hellmitre.homelinux.org");
	}
}
elsif ($arg eq "f")
{	
	if ($bool == 1)
	{
		print "Mounting minus22 filesystem locally...\n";
		exec ("sshfs -p 117 hlmtre\@minus22:/ /home/hlmtre/remote");
	}
	else
	{
		print "Mounting minus22 filesystem internetishly...\n";
		exec ("sshfs -p 117 hlmtre\@hellmitre.homelinux.org:/ /home/hlmtre/remote");
	}
}
return 0;
