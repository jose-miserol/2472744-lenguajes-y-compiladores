#include <stdio.h>

// Programa de prueba para el traductor lexico
entero main() {
    entero contador = 0;
    flotante promedio = 15.5;
    caracter letra = 'K';

    // Ciclo para contar
    para (entero i = 0; i < 5; i++) {
        si (i % 2 == 0) {
            contador++;
        } sino {
            continuar;
        }
    }

    /* Ciclo mientras el contador sea mayor a cero */
    mientras (contador > 0) {
        contador--;
        si (contador == 1) {
            romper;
        }
    }

    retornar 0;
}