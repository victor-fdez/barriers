#!/usr/bin/perl


use Net::SSH::Perl;

my $username = "vfernandez3";
my $password = "###########";
my $ssh = Net::SSH::Perl->new($host);
my $localDir = "./"
my $remote = "jinx-login.gatech.edu";
my $remoteDir = "cs6210_project";

#process arguments
sub main
{

}
#send all files in this directory to the server
sub sendFilesToServer
{
	my $dir = "./";
	opendir(DIR, $dir) or die $!;
	while(my $localFile = readdir(DIR))
	{
		#sftp file to the remote server
			
	}
	print "--> sent all files correctly to remote ".$remote."\n";
	
}
#compiler job on the server
sub compileFilesOnServer
{
}
#run a job on the server
sub runOnServer
{

}
#check periodically whether executable has finished running
sub statusOfRun
{

}
