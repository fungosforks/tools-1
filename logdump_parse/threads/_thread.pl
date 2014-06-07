#!perl

$i=0;
open (RD, "debug1.log");
while(<RD>){ chomp;
    $i++;

    if ($_=~ /^[^ ]+ T#(\d+) /) {
        $id=$1;
        open (WR, ">>thread_$id.log");
        print WR $_."\n";
        close(WR);
    };

    if (0) {
        print "Error! Line: ".$i."\n";
        exit(1);
    };

};
close(RD);
