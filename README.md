# Computación Paralela y Distribuida
## Trabajo Práctico - OpenMP

Repositorio correspondiente al trabajo práctico sobre OpenMP de la materia Computación Paralela y Distribuida.

Para mantener la simplicidad del código, cada versión paralela de cada problema se desarrolló en un archivo independiente.

### Estructura del proyecto
El presente proyecto está organizado en las siguientes carpetas:

- `data`: contiene los archivos de matrices generados por el script `test.sh` para que los programas de multiplicación de matrices utilicen como entrada.
- `doc`: contiene la documentación del proyecto (informe final).
- `results`: contiene los resultados crudos y procesados de los experimentos, junto con notebooks para el análisis de los resultados
- `src`: contiene el código fuente de los distintos programas.

### Versiones disponibles

Cálculo de número Pi
- utilizando cualquier directiva excepto “omp reduction” (`integration_no_reduction`)
- utilizando “omp parallel for” en conjunto con “omp reduction” (`integration`)
- aplicando la noción de scheduling (`integration_scheduling`)

Multiplicación de matrices
- utilizando cualquier directiva excepto “omp reduction” (`matrix_no_reduction`)
- utilizando “omp parallel for” en conjunto con “omp reduction” (`matrix`)
- aplicando la noción de scheduling (`matrix_scheduling`)

### Compilar y ejecutar individualmente

Para compilar alguno de los seis programas anteriormente mencionados, ejecutar el comando
```sh
make <<programa>>
```

El archivo binario se encontrará en la carpeta `bin`. Para ejecutarlo:
```sh
./bin/<<programa>>.exe $PARAMETROS
```

#### Parámetros
*integration*
- `num_threads` cantidad de threads
- `num_steps` cantidad de pasos


*integration_no_reduction*

- `num_threads` cantidad de threads
- `num_steps` cantidad de pasos

*integration_scheduling*

- `num_threads` cantidad de threads
- `num_steps` cantidad de pasos
- `schedule` tipo de schedule (1=estático, 2=dinámico)
- `schedule_chunk_size` tamaño del chunk


*matrix*

- `num_threads` cantidad de threads
- `matrix_a_filename` ubicación del archivo correspondiente a la matriz a
- `matrix_b_filename` ubicación del archivo correspondiente a la matriz b

*matrix_no_reduction*

- `num_threads` cantidad de threads
- `matrix_a_filename` ubicación del archivo correspondiente a la matriz a
- `matrix_b_filename` ubicación del archivo correspondiente a la matriz b

*matrix_scheduling*

- `num_threads` cantidad de threads
- `matrix_a_filename` ubicación del archivo correspondiente a la matriz a
- `matrix_b_filename` ubicación del archivo correspondiente a la matriz b
- `schedule` tipo de schedule (1=estático, 2=dinámico)
- `schedule_chunk_size` tamaño del chunk

### Compilar y ejecutar un archivo automáticamente

El script `run.sh` automatiza el compilado y ejecución de las distintas versiones de aplicaciones disponibles. En particular, para la aplicación de multiplicación de matrices genera los archivos correspondientes a las matrices con el tamaño indicado por parámetros y números aleatorios.

``` sh
run.sh <<programa>> <<número_de_threads>> <<...parámetros_programa>>
```

### Ejecutar en lote
El script `test.sh` permite ejecutar `x` cantidad de veces el mismo programa con la misma entrada (para obtener un tiempo de ejecución promedio estadísticamente significativo) y distintos threads (para analizar cómo se comporta la performance conforme el nivel de paralelismo aumenta).

``` sh
test.sh <<programa>> <<número_de_ejecuciones>> <<cantidad_máxima_de_threads>> <<programa>> <<...parámetros_programa>>
```

Asimismo, el script `test_scheduling.sh` además de permitir ejecutar `x` cantidad de veces el mismo programa con la misma entrada y distintos threads, ejecuta las aplicaciones variando el tipo de scheduling (estático y dinámico) y el tamaño de chunks (inicialmente establecido en 10, 50, 100, 500, 1000 y 5000). Los parámetros que admite son los mismos al script `test.sh`.

``` sh
test_scheduling.sh <<programa>> <<número_de_ejecuciones>> <<cantidad_máxima_de_threads>> <<programa>> <<...parámetros_programa>>
```
