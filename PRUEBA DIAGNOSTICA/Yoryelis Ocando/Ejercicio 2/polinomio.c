/*
 * ============================================================
 * Ejercicio 2 (C): coeficientes de (x+1)^n con el triangulo de Pascal
 *
 *   1) Compara las filas 0 a 7 del triangulo
 *   2) Genera varios polinomios, muestra el calculo por pasos de f(x)
 *      y comprueba el resultado contra la potencia calculada directamente.
 *   3) Mide el tiempo de generar los polinomios para n = 0..100 y guarda
 *      el resultado en un archivo txt.
 * ============================================================
 */

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned __int128 u128;

#define N_BENCHMARK 100
#define REPETICIONES 1000
#define ARCHIVO_RESULTADOS "resultados_ejercicio2_c.txt"

/* Filas 0 a 7 del triangulo de Pascal */
static const int TABLA_ENUNCIADO[8][8] = {
    {1},
    {1, 1},
    {1, 2, 1},
    {1, 3, 3, 1},
    {1, 4, 6, 4, 1},
    {1, 5, 10, 10, 5, 1},
    {1, 6, 15, 20, 15, 6, 1},
    {1, 7, 21, 35, 35, 21, 7, 1}
};

/* Casos de evaluacion: se calcula (x+1)^n para cada pareja. */
typedef struct {
    int n;
    int x;
} CasoEvaluacion;

static const CasoEvaluacion CASOS[] = {
    {0, 3}, {1, 2}, {2, 5}, {5, 2}, {7, 3}, {10, 4}
};
#define TOTAL_CASOS ((int)(sizeof CASOS / sizeof CASOS[0]))

static void escribir_u128(FILE *f, u128 v) {
    char buf[64];
    int i = 63;
    buf[i] = '\0';
    if (v == 0) { fputc('0', f); return; }
    while (v > 0) { buf[--i] = (char)('0' + (int)(v % 10)); v /= 10; }
    fputs(buf + i, f);
}

/* Devuelve la fila n del triangulo (n+1 coeficientes) en memoria dinamica. */
static u128 *generar_fila(int n) {
    u128 *fila = malloc(sizeof(u128));
    if (!fila) return NULL;
    fila[0] = 1;
    for (int r = 1; r <= n; r++) {
        u128 *nueva = malloc((r + 1) * sizeof(u128));
        if (!nueva) { free(fila); return NULL; }
        nueva[0] = nueva[r] = 1;
        for (int j = 1; j < r; j++) nueva[j] = fila[j - 1] + fila[j];
        free(fila);
        fila = nueva;
    }
    return fila;
}

/* Genera todo el triangulo de 0 a n. Devuelve un arreglo de filas dinamico. */
static u128 **generar_triangulo(int n) {
    u128 **tri = malloc((n + 1) * sizeof(u128 *));
    if (!tri) return NULL;
    for (int r = 0; r <= n; r++) {
        tri[r] = malloc((r + 1) * sizeof(u128));
        if (!tri[r]) { for (int k = 0; k < r; k++) free(tri[k]); free(tri); return NULL; }
        tri[r][0] = tri[r][r] = 1;
        for (int j = 1; j < r; j++) tri[r][j] = tri[r - 1][j - 1] + tri[r - 1][j];
    }
    return tri;
}

static void liberar_triangulo(u128 **tri, int n) {
    for (int r = 0; r <= n; r++) free(tri[r]);
    free(tri);
}

static void mostrar_polinomio(const u128 *c, int n) {
    printf("(x+1)^%d = ", n);
    for (int k = 0; k <= n; k++) {
        int p = n - k;
        if (k > 0) printf(" + ");
        if (p == 0) escribir_u128(stdout, c[k]);
        else {
            if (c[k] != 1) escribir_u128(stdout, c[k]);
            if (p == 1) printf("x"); else printf("x^%d", p);
        }
    }
    printf("\n");
}

/* Prueba 1: las filas generadas deben coincidir con la tabla del enunciado. */
static int probar_tabla_enunciado(void) {
    int aprobadas = 0;

    printf("========================================\n");
    printf("  Prueba 1 - Filas del enunciado (n = 0..7)\n");
    printf("========================================\n");

    for (int n = 0; n <= 7; n++) {
        u128 *fila = generar_fila(n);
        int coincide = (fila != NULL);

        for (int k = 0; coincide && k <= n; k++) {
            if (fila[k] != (u128)TABLA_ENUNCIADO[n][k]) coincide = 0;
        }

        printf("Fila n = %d: %s\n", n, coincide ? "CORRECTA" : "INCORRECTA");
        if (coincide) aprobadas++;
        free(fila);
    }

    printf("Filas correctas: %d de 8\n", aprobadas);
    return aprobadas == 8;
}

/* Prueba 2: genera el polinomio, evalua por pasos y compara con la potencia directa. */
static int probar_evaluacion(int n, int x) {
    u128 *c = generar_fila(n);
    u128 suma = 0;
    u128 directo = 1;
    u128 *potencias;

    if (!c) { printf("Error de memoria.\n"); return 0; }

    /* Potencias x^0, x^1, ..., x^n en memoria dinamica. */
    potencias = malloc((n + 1) * sizeof(u128));
    if (!potencias) { free(c); printf("Error de memoria.\n"); return 0; }
    potencias[0] = 1;
    for (int i = 1; i <= n; i++) potencias[i] = potencias[i - 1] * (u128)x;

    /* Valor esperado: (x+1)^n multiplicando directamente. */
    for (int i = 0; i < n; i++) directo *= (u128)(x + 1);

    printf("\n----------------------------------------\n");
    printf("n = %d, x = %d\n", n, x);
    printf("a) Coeficientes: ");
    for (int k = 0; k <= n; k++) { escribir_u128(stdout, c[k]); printf(" "); }
    printf("\n   Polinomio: ");
    mostrar_polinomio(c, n);

    printf("b) Calculo por pasos:\n");
    for (int k = 0; k <= n; k++) {
        int p = n - k;
        u128 termino = c[k] * potencias[p];
        suma += termino;
        printf("   Paso %d: ", k + 1);
        escribir_u128(stdout, c[k]);
        printf(" * (%d)^%d = ", x, p);
        escribir_u128(stdout, termino);
        printf(" | acumulado = ");
        escribir_u128(stdout, suma);
        printf("\n");
    }

    printf("   f(%d) segun el polinomio = ", x);
    escribir_u128(stdout, suma);
    printf("\n   (x+1)^n calculado directo = ");
    escribir_u128(stdout, directo);
    printf("\n   Resultado: %s\n", suma == directo ? "CORRECTO" : "INCORRECTO");

    free(potencias);
    free(c);
    return suma == directo;
}

static double ahora_microsegundos(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

/* Prueba 3: mide la generacion de los polinomios para n = 0..100. */
static void medir_tiempo(void) {
    double inicio = ahora_microsegundos();
    u128 **tri = generar_triangulo(N_BENCHMARK);
    double una = ahora_microsegundos() - inicio;
    u128 central;

    if (!tri) { printf("Error de memoria.\n"); return; }
    central = tri[N_BENCHMARK][N_BENCHMARK / 2];
    liberar_triangulo(tri, N_BENCHMARK);

    inicio = ahora_microsegundos();
    for (int i = 0; i < REPETICIONES; i++) {
        tri = generar_triangulo(N_BENCHMARK);
        liberar_triangulo(tri, N_BENCHMARK);
    }
    double total = ahora_microsegundos() - inicio;

    FILE *archivo = fopen(ARCHIVO_RESULTADOS, "w");
    FILE *destinos[2] = { stdout, archivo };

    printf("\n");
    for (int d = 0; d < 2; d++) {
        if (!destinos[d]) continue;
        fprintf(destinos[d], "========================================\n");
        fprintf(destinos[d], "  Ejercicio 2 - Triangulo de Pascal (C)\n");
        fprintf(destinos[d], "========================================\n");
        fprintf(destinos[d], "Polinomios generados      : (x+1)^n, n = 0..%d (%d polinomios)\n",
                N_BENCHMARK, N_BENCHMARK + 1);
        fprintf(destinos[d], "Coeficiente central n=100 : ");
        escribir_u128(destinos[d], central);
        fprintf(destinos[d], "\n");
        fprintf(destinos[d], "----------------------------------------\n");
        fprintf(destinos[d], "1 generacion              : %10.3f microsegundos (%.6f milisegundos)\n",
                una, una / 1000.0);
        fprintf(destinos[d], "Promedio de %d generaciones : %10.3f microsegundos (%.6f milisegundos)\n",
                REPETICIONES, total / REPETICIONES, total / REPETICIONES / 1000.0);
        fprintf(destinos[d], "========================================\n");
    }

    if (archivo) {
        fclose(archivo);
        printf("\nResultado guardado en %s\n", ARCHIVO_RESULTADOS);
    }
}

int main(void) {
    int todo_correcto = 1;
    int aprobados = 0;

    if (!probar_tabla_enunciado()) todo_correcto = 0;

    printf("\n========================================\n");
    printf("  Prueba 2 - Evaluacion de f(x) = (x+1)^n\n");
    printf("========================================");
    for (int i = 0; i < TOTAL_CASOS; i++) {
        if (probar_evaluacion(CASOS[i].n, CASOS[i].x)) aprobados++;
    }
    printf("\nEvaluaciones correctas: %d de %d\n", aprobados, TOTAL_CASOS);
    if (aprobados != TOTAL_CASOS) todo_correcto = 0;

    medir_tiempo();

    return todo_correcto ? 0 : 1;
}