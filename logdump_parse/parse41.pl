#!perl

use Win32::API;


#
# START CONFIG
#

$threadid = 3008;

#
# END CONFIG
#



#
# prepare file
#

open (RD, "debug1.log");
open (WR, ">debug1send.log");

while(<RD>){ chomp;
    $line = $_;

    if ($_=~ /^[^ ]+ T#$threadid /) {
        if ($_=~ /Logger AES dump from/) {
            print WR "\n\n";
        };
        print WR $line."\n";
    };

    if ($_ eq "") {
        print WR $line."\n";
    };
};

close(RD);
close(WR);


#
# parsing file
#

#
# load dll with 41 decoding functions
#
$fun = Win32::API->new('parse41.dll','int _sayhello@8(char *in, char *out)') or die $^E;


open (RD, "debug1send.log");
open (WR, ">debug1with41.log");

$sendcnt="0000";
$recvcnt="0000";
while(<RD>){ chomp;

    print WR "$_\n";


    # send
    if ($_=~ /Logger AES dump from 00914C97/){
        $sendcnt++;
        process_send(RD, WR, $sendcnt);
    };

    # recv
    if ($_=~ /Logger AES dump from 00915C16/){
        $recvcnt++;
        process_recv(RD, WR, $recvcnt);
    };
    # recv2
    if ($_=~ /Logger AES dump from 006A7F0D/){
        $recvcnt++;
        process_recv(RD, WR, $recvcnt);
    };


};

close(RD);


exit(0);


#
# do_decode
#
sub do_decode($) {
    my ($data) = @_;

    #$data = "00 A6 01 41 05 04 EF DF 03 01 10 00 00 02 00 01 ";
    #$data.= "7D 00 02 FF F3 01 05 03 41 02 00 01 02 00 19 1C ";
    #$data.= "A5 D0 ";

    $outbuf = " "x64000;
    $ret = $fun->Call($data, $outbuf);
    $outbuf =~ s/\0.*$//;

    #print "$outbuf\n";
    #print "Len: $ret\n";

    return $outbuf;
};


#
# send
#
#16:10:14.171 T#1456 Logger AES dump from 00914C97 len 2F
#16:10:14.171 T#1456 Before: 
#16:10:14.171 T#1456 00 A6 01 41 05 04 EF DF 03 02 4A BF 00 00 02 00 
#16:10:14.171 T#1456 15 9A F2 E4 AC 07 05 2F 41 01 00 02 02 9B C0 
#16:10:14.171 T#1456 After: 
#
#
#
sub process_send($a,$b) {
    my ($RD, $WR, $sendcnt) = @_;

    $t = <$RD>; chomp($t);
    print $WR "$t\n";
    if ($t=~ /Before/) {
        # ok
    }else{
        print "Error, no 'before' tag\n";
        exit(1);
    };

    print "Start send$sendcnt processing\n";

    $text = "";
    $data = "";
    $datashow = "";
    $t = <$RD>; chomp($t);
    while( $t=~ /^[^ ]+ [^ ]+ [0-9a-fA-F]{2} / ){
        print $WR "$t\n";

        $t=~ /^[^ ]+ [^ ]+ (.+) $/;
        $hex = $1;
        $data = $data.$hex." ";
        $datashow = $datashow."|".$hex."|\n";

        $t = <$RD>; chomp($t);
    };

    print "End send$sendcnt processing\n";

    if (0) {
        chomp($datashow);
        print $WR "===\n";
        print $WR "$datashow\n";
    };

    $text = do_decode($data);
    chomp($text);
    print $WR "===\n";
    print $WR "PARAM send$sendcnt\n";
    print $WR "===\n";
    print $WR "$text\n";
    print $WR "===\n";

    print $WR "$t\n";

    #close($WR); exit(1);

    return 0;
};

#
# recv
#
#16:10:14.171 T#1456 Logger AES dump from 00915C16 len 15
#16:10:14.171 T#1456 Before: 
#16:10:14.171 T#1456 After: 
#16:10:14.171 T#1456 00 A6 01 41 03 04 EF DF 03 01 7D 00 00 02 00 04 
#16:10:14.171 T#1456 FA C6 03 63 B6 
#
#
#
sub process_recv($a,$b){
    my ($RD, $WR, $recvcnt) = @_;

    $t = <$RD>; chomp($t);
    print $WR "$t\n";
    while( !($t=~ /After/) ){
        $t = <$RD>; chomp($t);
        print $WR "$t\n";
    };

    print "Start recv$recvcnt processing\n";

    $data = "";
    $datashow = "";
    $t = <$RD>; chomp($t);
    while( $t=~ /^[^ ]+ [^ ]+ [0-9a-fA-F]{2} / ){
        print $WR "$t\n";

        $t=~ /^[^ ]+ [^ ]+ (.+) $/;
        $hex = $1;
        $data = $data.$hex." ";
        $datashow = $datashow."|".$hex."|\n";

        $t = <$RD>; chomp($t);
    };

    print "End recv$recvcnt processing\n";

    if (0) {
        chomp($datashow);
        print $WR "===\n";
        print $WR "$datashow\n";
    };

    $text = do_decode($data);
    chomp($text);
    print $WR "===\n";
    print $WR "PARAM recv$recvcnt\n";
    print $WR "===\n";
    print $WR "$text\n";
    print $WR "===\n";
    
    print $WR "$t\n";

    #close($WR); exit(1);

    return 0;
};

