#!/bin/sh

export OMP_NUM_THREADS=3

make matrix
./bin/matrix.exe