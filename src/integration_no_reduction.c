/******************************************************
    Computación Paralela y Distribuida - TP OpenMP
    Junio 2022

    Objetivo:   Desarrollar una versión paralela de la aplicación para calcular el número Pi
                utilizando OpenMP con cualquier directiva excepto “omp reduction”.

    Entrada:    1) Cantidad de threads
                2) Número de steps

    Salida:     Impresión por pantalla del número Pi

    Creado:     Mayo 2022

******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

static void print_inputs(double num_steps, int num_threads);

// Argumentos que se reciben por parámetros
static int NUM_THREADS_ARG = 1;
static int NUM_STEPS_ARG = 2;

// Programa principal
int main(int argc, char *argv[])
{
    int i, num_threads;
    double x, pi, sum = 0.0;
    static long num_steps;
    if (argc != 3)
    {
        printf("Uso : integration_no_reduction.exe <num_threads> <num_steps>\n");
        exit(-1);
    }
    num_threads = atoi(argv[NUM_THREADS_ARG]);
    num_steps = atof(argv[NUM_STEPS_ARG]);
    omp_set_num_threads(num_threads);
    // Muestro por pantalla los argumentos recibidos
    print_inputs(num_steps, num_threads);
    double step = 1.0 / (double)num_steps;
    double start = omp_get_wtime();
    // Paralelizo las iteraciones del ciclo entre los threads recibidos en num_threads
    #pragma omp parallel for private(x)
    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        // Como sum es una variable compartida entre los threads, aseguro que sea modificada por cada thread atómicamente
        #pragma omp atomic
        sum = sum + 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    // Muestro los resultados por pantalla
    printf("Tiempo de ejecucion: %1.20lf\n", omp_get_wtime() - start);
    printf("---Resultados---\n");
    printf("Numero Pi: %1.15lf\n", pi);
    return 0;
}

void print_inputs(double num_steps, int num_threads)
{
    printf("---Cálculo número Pi---\n");
    printf("Threads: %d\n", num_threads);
    printf("Pasos: %f\n", num_steps);
    printf("Variante del problema: cualquier directiva excepto \"omp reduction\"\n");
}