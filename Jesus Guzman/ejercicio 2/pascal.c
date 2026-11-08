#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// ============================================================================
// DEFINICION: u128
// Entero sin signo de 128 bits para soportar valores de hasta 3.4 x 10^38.
// Requerido para n=100 donde los coeficientes centrales superan 10^29.
// ============================================================================
typedef unsigned __int128 u128;

// Funcion para serializar en texto e imprimir numeros de 128 bits
void imprimir_u128(u128 n, FILE* flujo) {
    if (n == 0) { fprintf(flujo, "0"); return; }
    char buf[50];
    int i = 0;
    while (n > 0) {
        buf[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        fputc(buf[j], flujo);
    }
}

// ============================================================================
// FUNCION: generar_pascal
// Genera los coeficientes del Triangulo de Pascal usando MEMORIA DINAMICA.
// ============================================================================
u128* generar_pascal(int n) {
    // 1. Asignacion dinamica: reserva exacta para (n + 1) coeficientes
    u128* coef = (u128*)malloc((n + 1) * sizeof(u128));
    if (!coef) {
        perror("Fallo critico al asignar memoria dinamica");
        exit(1);
    }

    // 2. Construccion del Triangulo de Pascal
    coef[0] = 1;
    for (int i = 1; i <= n; i++) {
        coef[i] = 1;
        // Calculo en retroceso para optimizar espacio a O(n)
        for (int j = i - 1; j > 0; j--) {
            coef[j] = coef[j] + coef[j - 1];
        }
    }
    return coef; // Retorna puntero a memoria dinamica en heap
}

// ============================================================================
// FUNCION: mostrar_polinomio
// Imprime la representacion algebraica de (x+1)^n
// ============================================================================
void mostrar_polinomio(u128* coef, int n) {
    printf("(x + 1)^%d = ", n);
    for (int i = 0; i <= n; i++) {
        int pot = n - i;
        if (coef[i] != 1 || pot == 0) {
            imprimir_u128(coef[i], stdout);
        }
        if (pot > 0) {
            printf("x");
            if (pot > 1) printf("^%d", pot);
        }
        if (i < n) printf(" + ");
    }
    printf("\n");
}

// ============================================================================
// FUNCION: evaluar_paso_a_paso
// Realiza el calculo secuencial f(x) = (x+1)^n mostrando cada termino
// ============================================================================
void evaluar_paso_a_paso(u128* coef, int n, double x) {
    printf("\n--- Calculo paso a paso para x = %.2f en (x + 1)^%d ---\n", x, n);
    double acumulado = 0.0;
    for (int i = 0; i <= n; i++) {
        int pot = n - i;
        double x_pot = pow(x, pot);
        double termino = (double)coef[i] * x_pot;
        acumulado += termino;

        printf("Paso %2d: Coef = ", i + 1);
        imprimir_u128(coef[i], stdout);
        printf(" | x^%d = %.4f | Termino = %.4f | Acumulado = %.4f\n", pot, x_pot, termino, acumulado);
    }
    printf("Resultado final f(%.2f) = %.4f\n", x, acumulado);
    printf("Comprobacion directa (%.2f + 1)^%d = %.4f\n\n", x, n, pow(x + 1.0, n));
}

int main() {
    printf("===================================================================\n");
    printf("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)\n");
    printf("  CATEDRA: LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA\n");
    printf("  EJERCICIO 2: POLINOMIO (x+1)^n Y PASCAL CON MEMORIA DINAMICA (C)\n");
    printf("  Estudiante: Jesus Guzman\n");
    printf("===================================================================\n\n");

    // 1. Mostrar primeros polinomios como en la tabla del enunciado
    printf("--- EJEMPLOS BASE DEL TRIANGULO DE PASCAL (n = 0 hasta 5) ---\n");
    for (int k = 0; k <= 5; k++) {
        u128* c = generar_pascal(k);
        mostrar_polinomio(c, k);
        free(c); // Liberacion inmediata
    }

    // 2. Evaluacion paso a paso para n = 4 con x = 2.0
    int n_demo = 4;
    double x_demo = 2.0;
    u128* coef_demo = generar_pascal(n_demo);
    evaluar_paso_a_paso(coef_demo, n_demo, x_demo);
    free(coef_demo);

    // 3. Demostracion para n = 100 y coeficiente central
    printf("--- DEMOSTRACION PARA n = 100 (101 ELEMENTOS EN MEMORIA DINAMICA) ---\n");
    u128* c100_demo = generar_pascal(100);
    printf("Coeficiente central (posicion 50) de (x+1)^100: ");
    imprimir_u128(c100_demo[50], stdout);
    printf("\n(Se comprueba la precision exacta de 30 digitos sin desbordamiento)\n\n");
    free(c100_demo);

    // 4. Medicion de rendimiento: 2,000 repeticiones para n = 100
    int repeticiones = 2000;
    printf("--- MEDICION DE TIEMPO EN C (n = 100 x %d ITERACIONES) ---\n", repeticiones);
    
    clock_t start = clock();
    for (int i = 0; i < repeticiones; i++) {
        u128* c100 = generar_pascal(100);
        free(c100); // Liberacion en cada ciclo para evitar fugas
    }
    clock_t end = clock();

    double tiempo_total_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    double tiempo_promedio_ms = tiempo_total_ms / repeticiones;

    printf("Tiempo total en C       : %.4f ms\n", tiempo_total_ms);
    printf("Tiempo promedio por fila: %.6f ms\n\n", tiempo_promedio_ms);

    // 5. Guardado en archivo de texto para comparativa
    FILE* f = fopen("tiempos_n100.txt", "w");
    if (f) {
        fprintf(f, "===================================================\n");
        fprintf(f, "COMPARATIVA DE TIEMPOS n=100 (C vs PYTHON)\n");
        fprintf(f, "Estudiante: Jesus Guzman\n");
        fprintf(f, "===================================================\n");
        fprintf(f, "[C] Repeticiones: %d\n", repeticiones);
        fprintf(f, "[C] Tiempo total: %.4f ms\n", tiempo_total_ms);
        fprintf(f, "[C] Tiempo promedio generacion n=100: %.6f ms\n", tiempo_promedio_ms);
        fprintf(f, "---------------------------------------------------\n");
        fclose(f);
        printf("-> Reporte de C guardado en 'tiempos_n100.txt'\n");
    }

    return 0;
}
