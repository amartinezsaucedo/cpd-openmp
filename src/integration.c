#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

static void print_inputs(double num_steps, int num_threads);

static int NUM_THREADS_ARG = 1;
static int NUM_STEPS_ARG = 2;

int main(int argc, char *argv[])
{
    int i, num_threads, variant;
    double x, pi, sum = 0.0;
    static long num_steps;
    if (argc != 3)
    {
        printf("Uso : integration.exe <num_threads> <num_steps>\n");
        exit(-1);
    }
    num_threads = atoi(argv[NUM_THREADS_ARG]);
    num_steps = atof(argv[NUM_STEPS_ARG]);
    omp_set_num_threads(num_threads);
    print_inputs(num_steps, num_threads);
    double step = 1.0 / (double)num_steps;
    double start = omp_get_wtime();
    #pragma omp parallel for private(x) reduction(+:sum)
    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    printf("Tiempo de ejecucion: %1.20lf\n", omp_get_wtime() - start);
    printf("---Resultados---\n");
    printf("Numero Pi: %1.15lf\n", pi);
    return 0;
}

void print_inputs(double num_steps, int num_threads) {
    printf("---Calculo numero Pi---\n");
    printf("Threads: %d\n", num_threads);
    printf("Pasos: %f\n", num_steps);
    printf("Variante del problema: utilizando \"omp parallel for\" en conjunto con \"omp reduction\"\n");
}