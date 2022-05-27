/******************************************************
    Computación Paralela y Distribuida - TP OpenMP
    Junio 2022

    Objetivo:   Desarrollar una versión paralela de la aplicación para multiplicar
                matrices utilizando OpenMP y aplicando la noción de scheduling.

    Entrada:    1) Cantidad de threads
                2) Nombre del archivo que contiene a la matriz A
                3) Nombre del archivo que contiene a la matriz B
				4) Tipo de scheduling (static=1, dynamic=2)
                5) Tamaño del chunk

    Salida:     Impresión por pantalla de la matriz resultado      
                                                                        
    Creado:     Mayo 2022

******************************************************/

#define _CRTDBG_MAP_ALLOC // Directiva para detectar memory leaks
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
// Header para detectar memory leaks
#include "crtdbg.h"

// Argumentos que se reciben por parámetros
static int NUM_THREADS_ARG = 1;
static int MATRIX_A_FILENAME_ARG = 2;
static int MATRIX_B_FILENAME_ARG = 3;
static int SCHEDULE_ARG = 4;
static int SCHEDULE_CHUNK_SIZE_ARG = 5;

static void print_inputs(int num_threads, int matrix_a_rows, int matrix_a_columns, int matrix_b_rows, int matrix_b_columns, int schedule, int schedule_chunk_size);

// Defino la estructura matrix con sus valores de enteros, filas y columnas
struct matrix
{
	int** matrix;
	int rows;
	int columns;
};

// Creo la estructura en memoria con la cantidad de filas y columnas especificada, sin inicializar
struct matrix* create_matrix(int rows, int columns)
{
	struct matrix* new_matrix = (struct matrix*)malloc(sizeof(struct matrix));
	new_matrix->matrix = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++)
	{
		*(new_matrix->matrix + i) = (int*)malloc(columns * sizeof(int));
	}
	new_matrix->rows = rows;
	new_matrix->columns = columns;
	return new_matrix;
}

// Abro el archivo correspondiente a una matriz y cargo sus valores en la estructura matrix correspondiente
// Estructura archivo matrix_a_<filas>x<columnas>.txt:
// cantidad_de_filas cantidad de columnas
// valor_1 valor_2 valor_3
// valor_4 valor_5 valor_6
// Ejemplo:
// matrix_a_2x3.txt
// 2 3
// 19 46 89 
// 27 89 17 
struct matrix* load_matrix_from_file(const char* filename)
{
	FILE* file;
	fopen_s(&file, filename, "rt");
	int rows, columns;
	fscanf_s(file, "%d", &rows);
	fscanf_s(file, "%d", &columns);
	struct matrix* new_matrix = create_matrix(rows, columns);
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

// Multiplico la matriz A de tamaño filas_Axcolumnas_A con la matriz B de tamaño filas_Bxcolumnas_B,
// obteniendo una matriz resultado de tamaño filas_Axcolumnas_B
struct matrix* multiply_matrices(const struct matrix* matrix_a, const struct matrix* matrix_b)
{
	struct matrix* result_matrix = create_matrix(matrix_a->rows, matrix_b->columns);
	int i, j, k, l, result;
	// Paralelizo las iteraciones del ciclo entre los threads recibidos en num_threads.
    // Como tengo 3 ciclos anidados, colapso los primeros 2 (paralelizándolos para evitar un escenario de paralelismo 
    // anidado) ya que en el tercero debo reinicializar el valor de result a 0 (para calcular el valor correspondiente a 
    // través de las filas y columnas). Como result es una variable compartida por los threads, cada uno tiene una copia 
	// local privada, pero al final de la región parallel todos aplican la suma y así actualizan el valor de result para
	// para una posición [i, j] de la matriz resultado. Por último, configuro el tipo y cantidad de bloques de iteraciones
	// que cada thread procesará según lo recibido como parámetros
    #pragma omp parallel for private(i, j, k, l) reduction(+: result) collapse(2) schedule(runtime)
	for (i = 0; i < matrix_a->rows; i++)
	{
		for (j = 0; j < matrix_b->columns; j++)
		{
			result = 0;
			for (k = 0, l = 0; k < matrix_a->columns && l < matrix_b->rows; k++, l++)
			{
				result += *(*(matrix_a->matrix + i) + k) * *(*(matrix_b->matrix + l) + j);
			}
			*(*(result_matrix->matrix + i) + j) = result;
		}
	}
	return result_matrix;
}

// Muestro por pantalla los valores de una matriz
void display_matrix(const struct matrix* matrix)
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

// Libero la memoria asignada para la matriz
void free_memory(const struct matrix* matrix)
{
	for (int i = 0; i < matrix->rows; i++)
	{
		free(matrix->matrix[i]);
	}
	free(matrix->matrix);
	free(matrix);
}

// Programa principal
int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Establezco flags necesarios para detectar memory leaks
	int num_threads, schedule, schedule_chunk_size;
	char* matrix_a_filename, * matrix_b_filename;
	if (argc != 6)
	{
		printf("Uso : matrix_scheduling.exe <num_threads> <matrix_a_filename> <matrix_b_filename> <schedule> <schedule_chunk_size>\n");
		exit(-1);
	}
	num_threads = atoi(argv[NUM_THREADS_ARG]);
	matrix_a_filename = argv[MATRIX_A_FILENAME_ARG];
	matrix_b_filename = argv[MATRIX_B_FILENAME_ARG];
	schedule = atoi(argv[SCHEDULE_ARG]);
    schedule_chunk_size = atoi(argv[SCHEDULE_CHUNK_SIZE_ARG]);
	struct matrix* matrix_a = load_matrix_from_file(matrix_a_filename);
	struct matrix* matrix_b = load_matrix_from_file(matrix_b_filename);
	// Si no puedo realizar la multiplicación porque el número de columnas de A y de filas de B difieren, termino el programa
	if (matrix_a->columns != matrix_b->rows)
    { 
        printf("La cantidad de columnas de la matriz A debe coincidir con la cantidad de filas de la matriz B\n");
        exit(-1);
    }
	omp_set_num_threads(num_threads);
	omp_set_schedule(schedule, schedule_chunk_size);
	// Muestro por pantalla los argumentos recibidos 
	print_inputs(num_threads, matrix_a->rows, matrix_a->columns, matrix_b->rows, matrix_b->columns, schedule, schedule_chunk_size);
	double start = omp_get_wtime();
	struct matrix* result = multiply_matrices(matrix_a, matrix_b);
	// Muestro los resultados por pantalla
	printf("Tiempo de ejecucion: %1.20lf\n", omp_get_wtime() - start);
	printf("---Resultados---\n");
	display_matrix(result);
	// Libero la memoria asignada a las distintas matrices
	free_memory(matrix_a);
	free_memory(matrix_b);
	free_memory(result);
	// Dumpeo memory leaks detectados
	_CrtDumpMemoryLeaks();
	return 0;
}

void print_inputs(int num_threads, int matrix_a_rows, int matrix_a_columns, int matrix_b_rows, int matrix_b_columns, int schedule, int schedule_chunk_size)
{
	printf("---Multiplicacion de matrices---\n");
	printf("Threads: %d\n", num_threads);
	printf("Schedule: %s\n", schedule == 1 ? "static" : "dynamic");
    printf("Tamaño de chunk: %d\n", schedule_chunk_size);
	printf("Dimension matriz A: %dx%d\n", matrix_a_rows, matrix_a_columns);
	printf("Dimension matriz B: %dx%d\n", matrix_b_rows, matrix_b_columns);
	printf("Variante del problema: utilizando \"omp parallel for\" en conjunto con \"omp reduction\" y scheduling\n");
}
