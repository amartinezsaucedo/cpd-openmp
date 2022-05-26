#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include "crtdbg.h"

static int NUM_THREADS_ARG = 1;
static int MATRIX_A_FILENAME_ARG = 2;
static int MATRIX_B_FILENAME_ARG = 3;

static void print_inputs(int num_threads, int matrix_a_rows, int matrix_a_columns, int matrix_b_rows, int matrix_b_columns);

struct matrix
{
    int **matrix;
    int rows;
    int columns;
};

struct matrix *create_matrix(int rows, int columns)
{
    struct matrix *new_matrix = (struct matrix *)malloc(sizeof(struct matrix));
    new_matrix->matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        *(new_matrix->matrix + i) = (int *)malloc(columns * sizeof(int));
    }
    new_matrix->rows = rows;
    new_matrix->columns = columns;
    return new_matrix;
}

struct matrix *load_matrix_from_file(const char *filename)
{
    FILE *file;
    fopen_s(&file, filename, "rt");
    int rows, columns;
    fscanf_s(file, "%d", &rows);
    fscanf_s(file, "%d", &columns);
    struct matrix *new_matrix = create_matrix(rows, columns);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            fscanf_s(file, "%d", (*(new_matrix->matrix + i) + j));
        }
    }
    fclose(file);
    return new_matrix;
}

struct matrix *multiply_matrices(const struct matrix *matrix_a, const struct matrix *matrix_b)
{
    if (matrix_a->columns != matrix_b->rows)
    {
        return NULL;
    }
    struct matrix *result_matrix = create_matrix(matrix_a->rows, matrix_b->columns);
    int i, j, k, l, result;
#pragma omp parallel for private(i, j, k, l, result) collapse(2)
    for (i = 0; i < matrix_a->rows; i++)
    {
        for (j = 0; j < matrix_b->columns; j++)
        {
            result = 0;
            for (k = 0, l = 0, result = 0; k < matrix_a->columns && l < matrix_b->rows; k++, l++)
            {
                result += *(*(matrix_a->matrix + i) + k) * *(*(matrix_b->matrix + l) + j);
            }
            *(*(result_matrix->matrix + i) + j) = result;
        }
    }
    return result_matrix;
}

void display_matrix(const struct matrix *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->columns; j++)
        {
            printf("%d\t", *(*(matrix->matrix + i) + j));
        }
        printf("\n");
    }
}

void free_memory(const struct matrix *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    free(matrix);
}

int main(int argc, char *argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    int num_threads;
    char *matrix_a_filename, *matrix_b_filename;
    if (argc != 4)
    {
        printf("Uso : matrix_no_reduction.exe <num_threads> <matrix_a_filename> <matrix_b_filename>\n");
        exit(-1);
    }
    num_threads = atoi(argv[NUM_THREADS_ARG]);
    matrix_a_filename = argv[MATRIX_A_FILENAME_ARG];
    matrix_b_filename = argv[MATRIX_B_FILENAME_ARG];
    struct matrix *matrix_a = load_matrix_from_file(matrix_a_filename);
    struct matrix *matrix_b = load_matrix_from_file(matrix_b_filename);
    omp_set_num_threads(num_threads);
    print_inputs(num_threads, matrix_a->rows, matrix_a->columns, matrix_b->rows, matrix_b->columns);
    double start = omp_get_wtime();
    struct matrix *result = multiply_matrices(matrix_a, matrix_b);
    printf("Tiempo de ejecucion: %1.20lf\n", omp_get_wtime() - start);
    printf("---Resultados---\n");
    display_matrix(result);
    free_memory(matrix_a);
    free_memory(matrix_b);
    free_memory(result);
    _CrtDumpMemoryLeaks();
    return 0;
}

void print_inputs(int num_threads, int matrix_a_rows, int matrix_a_columns, int matrix_b_rows, int matrix_b_columns)
{
    printf("---Multiplicacion de matrices---\n");
    printf("Threads: %d\n", num_threads);
    printf("Dimension matriz A: %dx%d\n", matrix_a_rows, matrix_a_columns);
    printf("Dimension matriz B: %dx%d\n", matrix_b_rows, matrix_b_columns);
    printf("Variante del problema: cualquier directiva excepto \"omp reduction\"\n");
}
