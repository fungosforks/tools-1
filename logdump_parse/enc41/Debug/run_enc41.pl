#!/usr/bin/perl

use Win32::API;
#use warnings;


$fun = Win32::API->new('C:\\vcnet_proj\\enc41\\Debug\\enc41.dll','int _sayhello@8(char *lol, char *outbuf)') or die $^E;

$data = "00 A6 01 41 05 04 EF DF 03 01 10 00 00 02 00 01 ";
$data.= "7D 00 02 FF F3 01 05 03 41 02 00 01 02 00 19 1C ";
$data.= "A5 D0 ";


#$outbuf = " "x64000;
#$ret = $fun->Call($data, $outbuf);
#$outbuf =~ s/\0.*$//;
#print "$outbuf\n";


$outbuf = " "x64000;
$ret = $fun->Call($data, $outbuf);
$outbuf =~ s/\0.*$//;

print "===\n";
print "$outbuf\n";
print "===\n";

print "Len: $ret\n";
