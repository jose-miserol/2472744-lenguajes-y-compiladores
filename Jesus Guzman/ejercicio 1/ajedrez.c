#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#if defined(_MSC_VER)
    #define strdup _strdup
#endif

// ============================================================================
// ESTRUCTURA: ResultadoValidacion
// Encapsula el veredicto booleano y el mensaje diagnostico formateado.
// ============================================================================
typedef struct {
    int es_valido;
    char mensaje[256];
} ResultadoValidacion;

// ============================================================================
// FUNCION: validar_movimientos
// Analiza secuencialmente cada salto del caballo a partir de la casilla Ak.
// ============================================================================
ResultadoValidacion validar_movimientos(const char* ak, char* movimientos[], int total) {
    ResultadoValidacion res;
    res.es_valido = 1;
    strcpy(res.mensaje, "movimiento CORRECTO.");

    // Mapeo inicial de coordenadas: columna [a-h] -> [0-7] y fila [1-8] -> [0-7]
    int col_prev = tolower(ak[0]) - 'a';
    int row_prev = ak[1] - '1';

    for (int i = 0; i < total; i++) {
        char* m = movimientos[i];
        if (!m || strlen(m) == 0) continue;

        // 1. ANALISIS LEXICO: Verificar identificador de la pieza (Debe ser 'K')
        if (m[0] != 'K') {
            res.es_valido = 0;
            snprintf(res.mensaje, sizeof(res.mensaje),
                     "movimiento INCORRECTO falla letra %c no pertenece a un caballo.", m[0]);
            return res; // Parada temprana inmediata
        }

        if (strlen(m) < 3) {
            res.es_valido = 0;
            snprintf(res.mensaje, sizeof(res.mensaje), "INCORRECTO falla en el movimiento %d.", i + 1);
            return res;
        }

        // Conversion de la casilla destino a indices numericos
        int col_act = tolower(m[1]) - 'a';
        int row_act = m[2] - '1';

        // 2. LIMITES DEL TABLERO: Verificar rango valido de 8x8 (0 a 7)
        if (col_act < 0 || col_act > 7 || row_act < 0 || row_act > 7) {
            res.es_valido = 0;
            snprintf(res.mensaje, sizeof(res.mensaje), "INCORRECTO falla en el movimiento %d.", i + 1);
            return res;
        }

        // 3. REGLA EN 'L': Distancia absoluta de 1x2 o 2x1 casillas
        int dx = abs(col_act - col_prev);
        int dy = abs(row_act - row_prev);

        if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) {
            res.es_valido = 0;
            snprintf(res.mensaje, sizeof(res.mensaje), "INCORRECTO falla en el movimiento %d.", i + 1);
            return res; // Parada temprana
        }

        // Actualizar posicion previa
        col_prev = col_act;
        row_prev = row_act;
    }
    return res;
}

// Cadena oficial de 500 movimientos completos del enunciado
const char* MOVIMIENTOS_500 = 
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
    "Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 "
    "Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 "
    "Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
    "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
    "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
;

// Funcion auxiliar para ejecutar y formatear pruebas unitarias
void ejecutar_prueba(int num, const char* ak, const char* cadena, const char* esperado) {
    char* copia = strdup(cadena);
    char* tokens[100];
    int total = 0;

    char* t = strtok(copia, " \t\r\n");
    while (t != NULL && total < 100) {
        tokens[total++] = t;
        t = strtok(NULL, " \t\r\n");
    }

    ResultadoValidacion r = validar_movimientos(ak, tokens, total);
    printf("[Prueba %d] Ak = %s | n = %2d\n", num, ak, total);
    printf("  Entrada : %s\n", cadena);
    printf("  Obtenido: %s\n", r.mensaje);
    printf("  Esperado: %s\n\n", esperado);

    free(copia);
}

int main() {
    printf("===================================================================\n");
    printf("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)\n");
    printf("  CATEDRA: LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA\n");
    printf("  EJERCICIO 1: VALIDACION DE MOVIMIENTOS DEL CABALLO (LENGUAJE C)\n");
    printf("  Estudiante: Jesus Guzman\n");
    printf("===================================================================\n\n");

    printf("--- CASOS DE PRUEBA OFICIALES DEL ENUNCIADO ---\n\n");
    
    ejecutar_prueba(1, "a1", "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO.");
    ejecutar_prueba(2, "a1", "Kc2 Kd4 Kb4 Kc5", "INCORRECTO falla en el movimiento 3.");
    ejecutar_prueba(3, "a2", "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO.");
    ejecutar_prueba(4, "a2", "Kc8 Ke4 Kf6 Ke8 Kg7", "INCORRECTO falla en el movimiento 1.");
    ejecutar_prueba(5, "a8", "Kb6 Kd7", "movimiento CORRECTO.");
    ejecutar_prueba(6, "a8", "Kb6 Kb7", "INCORRECTO falla en el movimiento 2.");
    ejecutar_prueba(7, "a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", "movimiento CORRECTO.");
    ejecutar_prueba(8, "a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4", "movimiento INCORRECTO falla letra Q no pertenece a un caballo.");
    ejecutar_prueba(9, "a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4", "movimiento INCORRECTO falla en el movimiento 9.");

    // Prueba de 500 movimientos verificados 10 veces
    printf("--- PRUEBA DE ESTRES Y MEDICION DE TIEMPO (500 MOVIMIENTOS x 10) ---\n");
    char* copia500 = strdup(MOVIMIENTOS_500);
    char* tokens500[600];
    int total500 = 0;

    char* tok = strtok(copia500, " \t\r\n");
    while (tok != NULL && total500 < 600) {
        tokens500[total500++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }

    const char* ak = "a2";
    ResultadoValidacion res500;

    clock_t start = clock();
    for (int rep = 0; rep < 10; rep++) {
        res500 = validar_movimientos(ak, tokens500, total500);
    }
    clock_t end = clock();

    double tiempo_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("Total de movimientos evaluados : %d\n", total500);
    printf("Posicion inicial               : %s\n", ak);
    printf("Resultado de la verificacion   : %s\n", res500.mensaje);
    printf("Tiempo total (10 repeticiones) : %.4f ms\n", tiempo_ms);
    printf("Tiempo promedio por iteracion  : %.4f ms\n\n", tiempo_ms / 10.0);

    // Guardado en archivo de texto
    FILE* f = fopen("tiempos_ejercicio1.txt", "w");
    if (f) {
        fprintf(f, "===================================================\n");
        fprintf(f, "RESULTADOS EJERCICIO 1: AJEDREZ (LENGUAJE C)\n");
        fprintf(f, "Estudiante: Jesus Guzman\n");
        fprintf(f, "===================================================\n");
        fprintf(f, "Movimientos evaluados: %d\n", total500);
        fprintf(f, "Posicion inicial: %s\n", ak);
        fprintf(f, "Estado: %s\n", res500.mensaje);
        fprintf(f, "Tiempo total (10 iteraciones): %.4f ms\n", tiempo_ms);
        fprintf(f, "Tiempo promedio por corrida: %.4f ms\n", tiempo_ms / 10.0);
        fprintf(f, "---------------------------------------------------\n");
        fclose(f);
        printf("-> Reporte guardado con exito en 'tiempos_ejercicio1.txt'\n");
    }

    free(copia500);
    return 0;
}
