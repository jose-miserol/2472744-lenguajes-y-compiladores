#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long* generar_pascal(int n) {
    long long *fila_anterior = (long long*) malloc(1 * sizeof(long long));
    if (!fila_anterior) return NULL;
    fila_anterior[0] = 1;

    for (int i = 1; i <= n; i++) {
        long long *fila_actual = (long long*) malloc((i + 1) * sizeof(long long));
        if (!fila_actual) {
            free(fila_anterior);
            return NULL;
        }
        fila_actual[0] = 1;
        fila_actual[i] = 1;
        for (int j = 1; j < i; j++) {
            fila_actual[j] = fila_anterior[j - 1] + fila_anterior[j];
        }
        free(fila_anterior);
        fila_anterior = fila_actual;
    }
    return fila_anterior;
}

void imprimir_polinomio(long long *coefs, int n) {
    printf("(x+1)^%d = ", n);
    for (int i = 0; i <= n; i++) {
        int potencia = n - i;
        if (i > 0) printf(" + ");
        if (potencia == 0) {
            printf("%lld", coefs[i]);
        } else if (coefs[i] == 1) {
            if (potencia == 1) printf("x");
            else printf("x^%d", potencia);
        } else {
            if (potencia == 1) printf("%lldx", coefs[i]);
            else printf("%lldx^%d", coefs[i], potencia);
        }
    }
    printf("\n");
}

void evaluar_por_pasos(long long *coefs, int n, long long x) {
    long long suma = 0;
    printf("\nEvaluacion paso a paso para x = %lld:\n", x);
    for (int i = 0; i <= n; i++) {
        int potencia = n - i;
        long long pot_x = 1;
        for (int p = 0; p < potencia; p++) {
            pot_x *= x;
        }
        long long termino = coefs[i] * pot_x;
        suma += termino;
        printf("  Paso %d: %lld * (%lld^%d) = %lld  (Suma acumulada = %lld)\n",
               i + 1, coefs[i], x, potencia, termino, suma);
    }
    printf("Resultado: f(%lld) = (%lld + 1)^%d = %lld\n", x, x, n, suma);
}

int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 6;
    long long x = (argc > 2) ? atoll(argv[2]) : 2;

    long long *coefs = generar_pascal(n);
    if (!coefs) {
        printf("Error asignando memoria dinamica.\n");
        return 1;
    }

    printf("--- EJERCICIO 2: POLINOMIO (x+1)^n CON TRIANGULO DE PASCAL EN C ---\n");
    printf("Coeficientes para (x+1)^%d:\n", n);
    for (int i = 0; i <= n; i++) {
        printf("%lld ", coefs[i]);
    }
    printf("\n");

    imprimir_polinomio(coefs, n);
    evaluar_por_pasos(coefs, n, x);
    free(coefs);

    /* Medicion de tiempo para n=100 */
    int iteraciones = 100;
    clock_t inicio = clock();
    for (int k = 0; k < iteraciones; k++) {
        long long *c = generar_pascal(100);
        free(c);
    }
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo C (%d iteraciones para n=100): %.6f s\n", iteraciones, tiempo);

    return 0;
}
