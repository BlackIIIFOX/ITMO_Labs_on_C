#!/usr/bin/perl -T
#
# Cat implementation.
 
use strict;
use warnings qw(FATAL all);

Main(@ARGV);

sub Main
{
	# perl -T lab1_2.pl TestFiles/file1.txt - TestFiles/file2.txt
	if ($#ARGV == -1)
	{
		print while(<STDIN>);
	}
	else
	{
		foreach my $argnum (0 .. $#ARGV) {
			HandleStreamAsCat($ARGV[$argnum]);
		}
	}
}

sub HandleStreamAsCat
{
	my ($streamName) = $_[0];

	if ($streamName eq "-")
	{
		print while(<STDIN>);
	}
	else
	{
		open F, "$streamName" or die $!;
		while (<F>) {
		    print $_;
		}
		close F;
	}
}