#!/bin/bash
for THREADS in 1 2 3 4
do
    OMP_NUM_THREADS=$THREADS
	./bench
done