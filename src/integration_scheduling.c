/******************************************************
    Computación Paralela y Distribuida - TP OpenMP
    Junio 2022

    Objetivo:   Desarrollar una versión paralela de la aplicación para calcular el número Pi
                utilizando OpenMP y aplicando la noción de scheduling.

    Entrada:    1) Cantidad de threads
                2) Número de steps
                3) Tipo de scheduling (static=1, dynamic=2)
                4) Tamaño del chunk

    Salida:     Impresión por pantalla del número Pi

    Creado:     Mayo 2022

******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

static void print_inputs(double num_steps, int num_threads, int schedule, int schedule_chunk_size);

// Argumentos que se reciben por parámetros
static int NUM_THREADS_ARG = 1;
static int NUM_STEPS_ARG = 2;
static int SCHEDULE_ARG = 3;
static int SCHEDULE_CHUNK_SIZE_ARG = 4;

// Programa principal
int main(int argc, char *argv[])
{
    int i, num_threads, schedule, schedule_chunk_size;
    double x, pi, sum = 0.0;
    static long num_steps;
    if (argc != 5)
    {
        printf("Uso : integration_scheduling.exe <num_threads> <num_steps> <schedule> <schedule_chunk_size>\n");
        exit(-1);
    }
    num_threads = atoi(argv[NUM_THREADS_ARG]);
    num_steps = atof(argv[NUM_STEPS_ARG]);
    schedule = atoi(argv[SCHEDULE_ARG]);
    schedule_chunk_size = atoi(argv[SCHEDULE_CHUNK_SIZE_ARG]);
    omp_set_num_threads(num_threads);
    omp_set_schedule(schedule, schedule_chunk_size);
    // Muestro por pantalla los argumentos recibidos
    print_inputs(num_steps, num_threads, schedule, schedule_chunk_size);
    double step = 1.0 / (double)num_steps;
    double start = omp_get_wtime();
    // Paralelizo las iteraciones del ciclo entre los threads recibidos en num_threads y
    // configuro el tipo y cantidad de bloques de iteraciones que cada uno procesará según
    // lo recibido por parámetros
    #pragma omp parallel for private(x) reduction(+:sum) schedule(runtime)
    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    // Muestro los resultados por pantalla
    printf("Tiempo de ejecucion: %1.20lf\n", omp_get_wtime() - start);
    printf("---Resultados---\n");
    printf("Numero Pi: %1.15lf\n", pi);
    return 0;
}

void print_inputs(double num_steps, int num_threads, int schedule, int schedule_chunk_size)
{
    printf("---Calculo numero Pi---\n");
    printf("Threads: %d\n", num_threads);
    printf("Schedule: %s\n", schedule == 1 ? "static" : "dynamic");
    printf("Tamaño de chunk: %d\n", schedule_chunk_size);
    printf("Pasos: %f\n", num_steps);
    printf("Variante del problema: utilizando \"omp parallel for\" en conjunto con \"omp reduction\" y scheduling\n");
}