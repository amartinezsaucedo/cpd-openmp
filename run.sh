#!/bin/sh

export OMP_NUM_THREADS=3

make integration
time ./bin/integration.exe 5 100000 1