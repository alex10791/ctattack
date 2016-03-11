#!/bin/bash

make cachemon_v2; 

reps=2

#make TARGET=cachemon_v2 run ARGS="127.0.0.1 4444 1 data.csv";

for i in `seq 1 2`;
do
	for j in `seq 1 $reps`;
	do
		#make TARGET=cachemon_v2 run ARGS="127.0.0.1 4445 $i data.$i.$j.csv";
		make TARGET=cachemon_v2 run ARGS="$i data.$i.$j.csv";
		#make TARGET=cachemon_v1 run ARGS="127.0.0.1 4445";
		#v data.csv data.$i.$j.csv
	done
done

for i in `seq 1 $reps`;
do
	echo "-----------------------------------"
	./datahist.py data.1.$i.csv data.2.$i.csv
done

