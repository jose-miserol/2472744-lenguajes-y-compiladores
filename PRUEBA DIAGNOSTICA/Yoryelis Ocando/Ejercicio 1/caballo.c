/*
 * ============================================================
 * Ejercicio 1: Movimientos de un caballo de ajedrez
 * Lenguaje: C
 *
 */

#if !defined(_WIN32)
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_MOVIMIENTOS 1024   /* maximo de movimientos que se pueden leer */
#define MAX_TEXTO 16384        /* tamano maximo de una cadena de movimientos */
#define REPETICIONES 10        /* veces que se repite la medicion */
#define ARCHIVO_RESULTADOS "resultados_ejercicio1_c.txt"

/* Secuencia de movimientos */
static const char *SECUENCIA =
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
    "Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
    "Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
    "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
    "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 "
    "Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
    "Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 "
    "Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4";

/* Casilla del tablero. */
typedef struct {
    int columna; /* 0 = a, 1 = b, ..., 7 = h */
    int fila;    /* 0 = 1, 1 = 2, ..., 7 = 8 */
} Posicion;

/* Caso de prueba. */
typedef struct {
    const char *inicial;   /* posicion inicial del caballo */
    int n;                 /* cantidad de movimientos */
    const char *cadena;    /* movimientos separados por espacio */
    const char *esperado;  /* salida que indica el enunciado */
} CasoPrueba;

static const CasoPrueba CASOS[] = {
    {"a1", 4,  "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO."},
    {"a1", 4,  "Kc2 Kd4 Kb4 Kc5", "INCORRECTO falla en el movimiento 3."},
    {"a2", 5,  "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO."},
    {"a2", 5,  "Kc8 Ke4 Kf6 Ke8 Kg7", "INCORRECTO falla en el movimiento 1."},
    {"a8", 2,  "Kb6 Kd7", "movimiento CORRECTO."},
    {"a8", 2,  "Kb6 Kb7", "INCORRECTO falla en el movimiento 2."},
    {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", "movimiento CORRECTO."},
    {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4",
     "INCORRECTO falla letra Q no pertenece a un caballo."},
    {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4",
     "INCORRECTO falla en el movimiento 9."}
};
#define TOTAL_CASOS ((int)(sizeof CASOS / sizeof CASOS[0]))

/* Convierte una casilla en columna y fila numericas. */
static int leer_casilla(const char *texto, Posicion *posicion) {
    char columna;
    char fila;

    if (texto == NULL || strlen(texto) != 2) {
        return 0;
    }

    columna = (char)tolower((unsigned char)texto[0]);
    fila = texto[1];

    if (columna < 'a' || columna > 'h') {
        return 0;
    }
    if (fila < '1' || fila > '8') {
        return 0;
    }

    posicion->columna = columna - 'a';
    posicion->fila = fila - '1';
    return 1;
}

/* Un caballo se mueve 1 casilla en una direccion y 2 en la otra, es decir, en forma de L. */
static int es_movimiento_caballo(Posicion actual, Posicion siguiente) {
    int diferencia_columna = abs(siguiente.columna - actual.columna);
    int diferencia_fila = abs(siguiente.fila - actual.fila);

    return (diferencia_columna == 1 && diferencia_fila == 2) ||
           (diferencia_columna == 2 && diferencia_fila == 1);
}

/*
 * Verifica una lista de movimientos. Se detiene en el primer error.
 * Devuelve 1 si todos son correctos y 0 si encuentra un error.
 */
static int verificar(Posicion actual, char *movimientos[], int n,
                     char *mensaje, size_t tamano) {
    int i;

    for (i = 0; i < n; i++) {
        const char *movimiento = movimientos[i];
        Posicion siguiente;

        if (movimiento == NULL || movimiento[0] == '\0') {
            snprintf(mensaje, tamano,
                     "INCORRECTO falla en el movimiento %d.", i + 1);
            return 0;
        }

        /* La primera letra debe ser K (caballo). */
        if (movimiento[0] != 'K') {
            snprintf(mensaje, tamano,
                     "INCORRECTO falla letra %c no pertenece a un caballo.",
                     movimiento[0]);
            return 0;
        }

        /* Despues de la K debe venir una casilla valida. */
        if (!leer_casilla(movimiento + 1, &siguiente)) {
            snprintf(mensaje, tamano,
                     "INCORRECTO falla en el movimiento %d.", i + 1);
            return 0;
        }

        /* El salto debe ser  del caballo. */
        if (!es_movimiento_caballo(actual, siguiente)) {
            snprintf(mensaje, tamano,
                     "INCORRECTO falla en el movimiento %d.", i + 1);
            return 0;
        }

        actual = siguiente;
    }

    snprintf(mensaje, tamano, "movimiento CORRECTO.");
    return 1;
}

/* Tiempo actual en segundos */
static double tiempo_segundos(void) {
#ifdef _WIN32
    LARGE_INTEGER frecuencia;
    LARGE_INTEGER contador;

    QueryPerformanceFrequency(&frecuencia);
    QueryPerformanceCounter(&contador);
    return (double)contador.QuadPart / (double)frecuencia.QuadPart;
#else
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#endif
}

/* Separa un texto en movimientos usando los espacios como separadores. */
static int separar_movimientos(char *texto, char *movimientos[], int maximo) {
    int cantidad = 0;
    char *token = strtok(texto, " \t\r\n");

    while (token != NULL && cantidad < maximo) {
        movimientos[cantidad] = token;
        cantidad++;
        token = strtok(NULL, " \t\r\n");
    }

    return cantidad;
}

/*
 * Corre todos los casos de prueba  y muestra si la
 * salida obtenida coincide con la esperada.
 * Devuelve la cantidad de casos que pasaron.
 */
static int ejecutar_pruebas(void) {
    int aprobados = 0;
    int i;

    printf("========================================\n");
    printf("  Ejercicio 1 - Pruebas del enunciado\n");
    printf("========================================\n");

    for (i = 0; i < TOTAL_CASOS; i++) {
        char copia[MAX_TEXTO];
        char *movimientos[MAX_MOVIMIENTOS];
        char mensaje[256];
        Posicion inicial;
        int cantidad;
        int coincide;

        leer_casilla(CASOS[i].inicial, &inicial);
        strncpy(copia, CASOS[i].cadena, sizeof copia - 1);
        copia[sizeof copia - 1] = '\0';
        cantidad = separar_movimientos(copia, movimientos, MAX_MOVIMIENTOS);

        if (cantidad < CASOS[i].n) {
            snprintf(mensaje, sizeof mensaje,
                     "Faltan movimientos: se esperaban %d y hay %d.",
                     CASOS[i].n, cantidad);
        } else {
            verificar(inicial, movimientos, CASOS[i].n, mensaje, sizeof mensaje);
        }

        coincide = (strcmp(mensaje, CASOS[i].esperado) == 0);
        if (coincide) {
            aprobados++;
        }
   printf("\n [%s]  Prueba %d de %d\n",
               coincide ? "  OK  " : "FALLO ", i + 1, TOTAL_CASOS);
        printf("  Posicion inicial : %s\n", CASOS[i].inicial);
        printf("  Movimientos (%2d) : %s\n", CASOS[i].n, CASOS[i].cadena);
        printf("  Salida esperada  : %s\n", CASOS[i].esperado);
        printf("  Salida obtenida  : %s\n", mensaje);
        printf("  ----------------------------------------\n");
    }
 
    printf("\n========================================\n");
    printf("  Pruebas aprobadas: %d de %d\n", aprobados, TOTAL_CASOS);
    printf("========================================\n");
    return aprobados;
}

/* Escribe el reporte de tiempos */
static void escribir_reporte(FILE *salida, int cantidad, const char *mensaje,
                             const double tiempos[], double total) {
    int i;

    fprintf(salida, "========================================\n");
    fprintf(salida, "  Ejercicio 1 - Caballo de ajedrez (C)\n");
    fprintf(salida, "========================================\n");
    fprintf(salida, "Posicion inicial        : a2\n");
    fprintf(salida, "Movimientos verificados : %d\n", cantidad);
    fprintf(salida, "Repeticiones            : %d\n", REPETICIONES);
    fprintf(salida, "Resultado               : %s\n", mensaje);
    fprintf(salida, "----------------------------------------\n");

    for (i = 0; i < REPETICIONES; i++) {
        fprintf(salida, "Repeticion %2d : %10.3f microsegundos\n",
                i + 1, tiempos[i]);
    }

    fprintf(salida, "----------------------------------------\n");
    fprintf(salida, "Total    : %10.3f microsegundos (%.6f milisegundos)\n",
            total, total / 1000.0);
    fprintf(salida, "Promedio : %10.3f microsegundos\n", total / REPETICIONES);
    fprintf(salida, "========================================\n");
}

/* Mide 10 verificaciones de los 500 movimientos y guarda el resultado. */
static void ejecutar_medicion(void) {
    char copia[MAX_TEXTO];
    char *movimientos[MAX_MOVIMIENTOS];
    char mensaje[256];
    Posicion inicial;
    double tiempos[REPETICIONES];
    double total = 0.0;
    int cantidad;
    int i;
    FILE *archivo;

    leer_casilla("a2", &inicial);

    strncpy(copia, SECUENCIA, sizeof copia - 1);
    copia[sizeof copia - 1] = '\0';
    cantidad = separar_movimientos(copia, movimientos, MAX_MOVIMIENTOS);

    /* Verificacion previa para conocer el resultado. */
    verificar(inicial, movimientos, cantidad, mensaje, sizeof mensaje);

    for (i = 0; i < REPETICIONES; i++) {
        double inicio = tiempo_segundos();
        verificar(inicial, movimientos, cantidad, mensaje, sizeof mensaje);
        tiempos[i] = (tiempo_segundos() - inicio) * 1e6;
        total += tiempos[i];
    }

    printf("\n");
    escribir_reporte(stdout, cantidad, mensaje, tiempos, total);

    archivo = fopen(ARCHIVO_RESULTADOS, "w");
    if (archivo != NULL) {
        escribir_reporte(archivo, cantidad, mensaje, tiempos, total);
        fclose(archivo);
        printf("\nResultado guardado en %s\n", ARCHIVO_RESULTADOS);
    } else {
        printf("\nNo se pudo crear %s\n", ARCHIVO_RESULTADOS);
    }
}

int main(void) {
    int aprobados = ejecutar_pruebas();

    ejecutar_medicion();

    return (aprobados == TOTAL_CASOS) ? 0 : 1;
}