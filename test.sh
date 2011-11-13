#!/bin/bash

SEP="================================================================================"

runTests() {
	echo "Output files to $outdir"
	while read i; do
		echo -e "$SEP\n$SEP"
		echo "Running examples from " $i
		(cd "$i"
		find . -name '*.odc' | while read j; do
			echo $SEP
			echo "Running example $j from $i"
			out=${j/#\.\//}
			out=${out//\//_}
			echo "Output being sent to $out"
			out=$outdir/$out
			$odcread "$j" >"$out"
		done
		)
		echo
	done < 'paths.txt'
}

# create clean output directory
rm -rf out
mkdir out

odcread=`readlink -f odcread`
outdir=`readlink -f out`

runTests 2>&1 | tee out.txt
