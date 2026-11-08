#include <stdio.h>

// Programa de prueba para el traductor lexico
int main() {
    int contador = 0;
    float promedio = 15.5;
    char letra = 'K';

    // Ciclo para contar
    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) {
            contador++;
        } else {
            continue;
        }
    }

    /* Ciclo mientras el contador sea mayor a cero */
    while (contador > 0) {
        contador--;
        if (contador == 1) {
            break;
        }
    }

    return 0;
}