#!/bin/sh

ARGS=("$@")

NUMBER_OF_EXECUTIONS=$1
MAX_NUMBER_OF_THREADS=$2
PROGRAM=$3
PROGRAM_ARGS=${ARGS[@]:3}
declare -a schedule_types=(1 2)
declare -a chunk_sizes=(10 50 100 500 1000 5000)

make $PROGRAM

for threads in $(seq $MAX_NUMBER_OF_THREADS); do
    for i in $(seq $NUMBER_OF_EXECUTIONS); do
        for schedule in "${schedule_types[@]}"; do
            for chunk_size in "${chunk_sizes[@]}"; do
                sh ./run.sh $PROGRAM $threads $PROGRAM_ARGS $schedule $chunk_size
            done
        done
    done
done
