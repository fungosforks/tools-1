#!perl

open (RD, "debug1.log");
open (WR, ">debug1send.log");

while(<RD>){ chomp;
    $line = $_;

    if ($_=~ /^[^ ]+ T#1456 /) {
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
