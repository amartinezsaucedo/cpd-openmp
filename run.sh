#!/bin/sh

PROGRAM=$1
THREADS=$2
ARG_1=$3
ARG_2=$4
ARG_3=$5
ARG_4=$6
ARG_5=$7

MATRIX_A_FILENAME="data/matrix_a_${ARG_1}x${ARG_2}.txt"
MATRIX_B_FILENAME="data/matrix_b_${ARG_2}x${ARG_3}.txt"

if [[ $PROGRAM == matrix* ]]; then
    if [[ ! -e $MATRIX_A_FILENAME ]]; then
        exec 3>>$MATRIX_A_FILENAME
        echo "$ARG_1 $ARG_2" >&3
        for i in $(seq $ARG_1); do
            for j in $(seq $ARG_2); do printf "$(((RANDOM % 100) + 1)) "; done
            printf "\n"
        done >&3
        exec 3<&-
    fi

    if [[ ! -e $MATRIX_B_FILENAME ]]; then
        exec 4>>$MATRIX_B_FILENAME
        echo "$ARG_2 $ARG_3" >&4
        for i in $(seq $ARG_2); do
            for j in $(seq $ARG_3); do printf "$(((RANDOM % 100) + 1)) "; done
            printf "\n"
        done >&4
        exec 4<&-
    fi
    ARGS_STRING="data/matrix_a_${ARG_1}x${ARG_2}.txt data/matrix_b_${ARG_2}x${ARG_3}.txt ${ARG_4} ${ARG_5}"
fi

if [[ $PROGRAM == integration* ]]; then
    ARGS_STRING="${ARG_1} ${ARG_2} ${ARG_3}"
fi

./bin/$PROGRAM.exe $THREADS $ARGS_STRING | tee /dev/tty | echo $PROGRAM $THREADS $ARG_1 $ARG_2 $ARG_3 $ARG_4 $ARG_5 $(sed -n 's/.*Tiempo de ejecucion://p') >>results/results_$PROGRAM.txt
