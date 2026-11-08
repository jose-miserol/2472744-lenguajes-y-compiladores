/*
 * Ejercicio 2 (version C): coeficientes de (x+1)^n con el triangulo de Pascal.
 *
 * Uso: ./pascal <n> <x> [repeticiones]
 *
 * Hace tres cosas:
 *   a) genera los coeficientes de (x+1)^n y muestra el polinomio,
 *   b) evalua f(x) = (x+1)^n paso a paso con esos coeficientes,
 *   c) mide cuanto tarda en generar el triangulo para n dado.
 *
 * Memoria dinamica: el triangulo es un arreglo de punteros y cada fila
 * se reserva con malloc con el tamano justo (fila i tiene i+1 numeros).
 * Al final se libera todo con free.
 *
 * Los coeficientes de n=100 llegan a 1e29, mas de lo que cabe en un
 * unsigned long long (1.8e19). Por eso uso unsigned __int128 de gcc,
 * que llega hasta 3.4e38 y alcanza para n=100.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef unsigned __int128 numero_grande;

/* printf no sabe imprimir __int128, asi que lo convierto a texto a mano. */
void numero_a_texto(numero_grande valor, char *texto)
{
    char invertido[64];
    int cantidad_digitos = 0;
    int posicion;

    if (valor == 0) {
        strcpy(texto, "0");
        return;
    }

    /* Saco los digitos de derecha a izquierda dividiendo entre 10. */
    while (valor > 0) {
        invertido[cantidad_digitos] = (char)('0' + (int)(valor % 10));
        valor = valor / 10;
        cantidad_digitos++;
    }

    /* Los voy copiando al reves para que queden en el orden correcto. */
    for (posicion = 0; posicion < cantidad_digitos; posicion++) {
        texto[posicion] = invertido[cantidad_digitos - 1 - posicion];
    }
    texto[cantidad_digitos] = '\0';
}

/* Reserva y llena las filas 0..n del triangulo. Devuelve el arreglo de filas. */
numero_grande **generar_triangulo(int n)
{
    int numero_fila;
    int posicion;
    numero_grande **triangulo = malloc((n + 1) * sizeof(numero_grande *));

    if (triangulo == NULL) {
        return NULL;
    }

    for (numero_fila = 0; numero_fila <= n; numero_fila++) {
        triangulo[numero_fila] = malloc((numero_fila + 1) * sizeof(numero_grande));

        for (posicion = 0; posicion <= numero_fila; posicion++) {
            /* Los bordes de cada fila siempre valen 1. */
            if (posicion == 0 || posicion == numero_fila) {
                triangulo[numero_fila][posicion] = 1;
            } else {
                /* Cada numero interior es la suma de los dos que tiene encima. */
                triangulo[numero_fila][posicion] =
                    triangulo[numero_fila - 1][posicion - 1] +
                    triangulo[numero_fila - 1][posicion];
            }
        }
    }

    return triangulo;
}

void liberar_triangulo(numero_grande **triangulo, int n)
{
    int numero_fila;
    for (numero_fila = 0; numero_fila <= n; numero_fila++) {
        free(triangulo[numero_fila]);
    }
    free(triangulo);
}

/* Imprime el polinomio tipo x^3+3x^2+3x+1 a partir de la ultima fila. */
void imprimir_polinomio(numero_grande *coeficientes, int n)
{
    int posicion;
    int potencia;
    char texto[64];

    for (posicion = 0; posicion <= n; posicion++) {
        potencia = n - posicion;

        /* Un coeficiente 1 no se escribe delante de la x (x^2 y no 1x^2). */
        if (!(coeficientes[posicion] == 1 && potencia > 0)) {
            numero_a_texto(coeficientes[posicion], texto);
            printf("%s", texto);
        }

        if (potencia > 1) {
            printf("x^%d", potencia);
        } else if (potencia == 1) {
            printf("x");
        }

        if (posicion < n) {
            printf("+");
        }
    }
    printf("\n");
}

/* Estima (x+1)^n con double para saber si el resultado entra en 128 bits (~3.4e38). */
int resultado_cabe_en_128_bits(int n, long long x)
{
    double estimado = 1.0;
    int vuelta;

    for (vuelta = 0; vuelta < n; vuelta++) {
        estimado = estimado * ((double)x + 1.0);
    }
    return estimado < 1e37;
}

/* Calcula f(x) mostrando cada termino coeficiente * x^potencia. */
void evaluar_por_pasos(numero_grande *coeficientes, int n, long long x)
{
    int posicion;
    int potencia;
    int vuelta;
    numero_grande potencia_de_x;
    numero_grande valor_termino;
    numero_grande suma = 0;
    char texto_coeficiente[64];
    char texto_termino[64];
    char texto_suma[64] = "0";

    for (posicion = 0; posicion <= n; posicion++) {
        potencia = n - posicion;

        /* x^potencia calculado con multiplicaciones sucesivas. */
        potencia_de_x = 1;
        for (vuelta = 0; vuelta < potencia; vuelta++) {
            potencia_de_x = potencia_de_x * (numero_grande)x;
        }

        valor_termino = coeficientes[posicion] * potencia_de_x;
        suma = suma + valor_termino;

        numero_a_texto(coeficientes[posicion], texto_coeficiente);
        numero_a_texto(valor_termino, texto_termino);
        numero_a_texto(suma, texto_suma);
        printf("  paso %d: %s * %lld^%d = %s   (suma parcial = %s)\n",
               posicion + 1, texto_coeficiente, x, potencia, texto_termino, texto_suma);
    }

    printf("f(%lld) = %s\n", x, texto_suma);
}

/* Promedia el tiempo (en microsegundos) de generar el triangulo hasta n. */
double medir_generacion(int n, int repeticiones)
{
    int vuelta;
    double segundos;

#ifdef _WIN32
    LARGE_INTEGER freq, inicio, fin;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&inicio);
    for (vuelta = 0; vuelta < repeticiones; vuelta++) {
        numero_grande **triangulo = generar_triangulo(n);
        liberar_triangulo(triangulo, n);
    }
    QueryPerformanceCounter(&fin);
    segundos = (double)(fin.QuadPart - inicio.QuadPart) / (double)freq.QuadPart;
    return segundos / repeticiones * 1e6;
#else
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (vuelta = 0; vuelta < repeticiones; vuelta++) {
        numero_grande **triangulo = generar_triangulo(n);
        liberar_triangulo(triangulo, n);
    }
    clock_gettime(CLOCK_MONOTONIC, &fin);

    segundos = (double)(fin.tv_sec - inicio.tv_sec) +
               (double)(fin.tv_nsec - inicio.tv_nsec) / 1e9;
    return segundos / repeticiones * 1e6;
#endif
}

int main(int argc, char *argv[])
{
    int n = (argc > 1) ? atoi(argv[1]) : 5;
    long long x = (argc > 2) ? atoll(argv[2]) : 2;
    int repeticiones = (argc > 3) ? atoi(argv[3]) : 1000;
    int posicion;
    char texto[64];
    numero_grande **triangulo;

    /* Con n mayor a 120 los coeficientes ya no caben en 128 bits. */
    if (n < 0 || n > 120) {
        printf("n debe estar entre 0 y 120\n");
        return 1;
    }

    triangulo = generar_triangulo(n);
    if (triangulo == NULL) {
        printf("No hay memoria suficiente\n");
        return 1;
    }

    printf("n = %d\n", n);
    printf("Coeficientes:");
    for (posicion = 0; posicion <= n; posicion++) {
        numero_a_texto(triangulo[n][posicion], texto);
        printf(" %s", texto);
    }
    printf("\n(x+1)^%d = ", n);
    imprimir_polinomio(triangulo[n], n);

    /* Para x grande el resultado se sale de 128 bits. Lo aviso en vez de mostrar basura. */
    if (x >= 0 && resultado_cabe_en_128_bits(n, x)) {
        printf("\nCalculo de f(%lld) = (x+1)^%d por pasos:\n", x, n);
        evaluar_por_pasos(triangulo[n], n, x);
    } else {
        printf("\nEvaluacion omitida: x debe ser >= 0 y (x+1)^n debe caber en 128 bits.\n");
    }

    liberar_triangulo(triangulo, n);

    printf("\nTiempo promedio para generar n=%d (%d repeticiones): %.3f microsegundos\n",
           n, repeticiones, medir_generacion(n, repeticiones));
    return 0;
}
