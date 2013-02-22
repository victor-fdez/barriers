#!/usr/bin/perl

my $output=`mpirun -hostfile machinefile ./testMPI`;
print $output;
