#!/usr/bin/perl

# Collecting experimental data for sortbench program
# Assume machine with 8 cores, each having 2-way hyperthreading
$corecount = 8;
$hypercount = 2;

# Program
$prog = "./sortbench";

# Parameters
$lognele = 27;
$logmaxfrac = 14;
$logmaxpfrac = 4;

$nele = 1 << $lognele;

print "Sorting $nele values\n";
print "Rows: Parallel sorting threshold fraction\n";
print "Columns: Parallel partitioning threshold fraction\n";
print "\t";
for ($pf = 0; $pf <= $logmaxpfrac; $pf++) {
    $pfrac = 1 << $pf;
    print "$pfrac\t";
}
print "MLim\tHLim\n";

# Time for sequential sort
$stime = 0;

for ($f = 0; $f <= $logmaxfrac; $f++) {
    $frac = 1 << $f;
    print "$frac\t";
    for ($pf = 0; $pf <= $logmaxpfrac; $pf++) {
	if ($pf <= $f) {
	    $pfrac = 1 << $pf;
	    $result = `$prog -n $nele -f $frac -F $pfrac`;
	    if ($result =~ "([0-9]+\.[0-9]+) ") {
		$val = $1;
		if ($f == 0) {
		    $stime = $val;
		}
		print "$val\t";
	    }
	} else {
	    print "-\t";
	}
    }
    $mval = $stime / $corecount;
    $hval = $mval / $hypercount;
    print "$mval\t$hval\n";
}
