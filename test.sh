#!/bin/sh

ARGS=("$@")

NUMBER_OF_EXECUTIONS=$1
MAX_NUMBER_OF_THREADS=$2
PROGRAM=$3
PROGRAM_ARGS=${ARGS[@]:3}

make $PROGRAM

for threads in $(seq $MAX_NUMBER_OF_THREADS); do
    for i in $(seq $NUMBER_OF_EXECUTIONS); do
        sh ./run.sh $PROGRAM $threads $PROGRAM_ARGS
    done
done
