/*
 * Ejercicio 3: busca las palabras reservadas de C en un programa y las
 * traduce al espanol.
 *
 * Uso: ./traductor <archivo.c>
 *
 * Pasos:
 *   1. Medir el archivo y reservar con malloc un bloque del tamano justo.
 *   2. Leer todo el archivo a ese bloque (el programa queda en memoria).
 *   3. Recorrer el bloque caracter por caracter. Los comentarios, los
 *      textos entre comillas y los caracteres entre apostrofes se copian
 *      sin tocar: un "int" dentro de un printf no es una palabra reservada.
 *   4. Cada palabra fuera de esos casos se busca en la tabla de las 32
 *      palabras reservadas de C. Si esta, se escribe su traduccion.
 *   5. Mostrar el programa traducido y cuantas veces aparecio cada palabra.
 *
 * Las traducciones no llevan tildes ni enie para que el resultado se
 * pueda leer sin problemas en cualquier terminal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOTAL_PALABRAS 32

/* Palabra reservada de C, su traduccion y un contador de apariciones. */
struct palabra_reservada {
    const char *original;
    const char *traduccion;
    int apariciones;
};

struct palabra_reservada tabla[TOTAL_PALABRAS] = {
    {"auto", "automatico", 0},     {"break", "romper", 0},
    {"case", "caso", 0},           {"char", "caracter", 0},
    {"const", "constante", 0},     {"continue", "continuar", 0},
    {"default", "defecto", 0},     {"do", "hacer", 0},
    {"double", "doble", 0},        {"else", "sino", 0},
    {"enum", "enumeracion", 0},    {"extern", "externo", 0},
    {"float", "flotante", 0},      {"for", "para", 0},
    {"goto", "irA", 0},            {"if", "si", 0},
    {"int", "entero", 0},          {"long", "largo", 0},
    {"register", "registro", 0},   {"return", "retornar", 0},
    {"short", "corto", 0},         {"signed", "consigno", 0},
    {"sizeof", "tamano", 0},       {"static", "estatico", 0},
    {"struct", "estructura", 0},   {"switch", "segun", 0},
    {"typedef", "definirtipo", 0}, {"union", "union", 0},
    {"unsigned", "sinsigno", 0},   {"void", "vacio", 0},
    {"volatile", "volatil", 0},    {"while", "mientras", 0}
};

/* Devuelve la posicion de la palabra en la tabla, o -1 si no es reservada. */
int buscar_en_tabla(const char *palabra)
{
    int indice;
    for (indice = 0; indice < TOTAL_PALABRAS; indice++) {
        if (strcmp(palabra, tabla[indice].original) == 0) {
            return indice;
        }
    }
    return -1;
}

/* Carga el archivo completo en memoria dinamica. Quien llama debe hacer free. */
char *cargar_archivo(const char *nombre, long *tamano)
{
    FILE *archivo = fopen(nombre, "rb");
    char *contenido;

    if (archivo == NULL) {
        return NULL;
    }

    /* Voy al final para saber cuantos bytes tiene el archivo. */
    fseek(archivo, 0, SEEK_END);
    *tamano = ftell(archivo);
    rewind(archivo);

    /* El +1 es para el '\0' que cierra el texto. */
    contenido = malloc(*tamano + 1);
    if (contenido == NULL) {
        fclose(archivo);
        return NULL;
    }

    fread(contenido, 1, *tamano, archivo);
    contenido[*tamano] = '\0';
    fclose(archivo);
    return contenido;
}

/*
 * Recorre el programa cargado y escribe la version traducida en stdout.
 * Cuenta cada palabra reservada encontrada en la tabla.
 */
void traducir_programa(const char *texto, long tamano)
{
    long posicion = 0;

    while (posicion < tamano) {
        char actual = texto[posicion];

        /* Comentario de bloque: se copia hasta encontrar el cierre. */
        if (actual == '/' && posicion + 1 < tamano && texto[posicion + 1] == '*') {
            putchar(texto[posicion++]);
            putchar(texto[posicion++]);
            while (posicion < tamano &&
                   !(texto[posicion] == '*' && posicion + 1 < tamano && texto[posicion + 1] == '/')) {
                putchar(texto[posicion++]);
            }
            if (posicion < tamano) {
                putchar(texto[posicion++]);
                putchar(texto[posicion++]);
            }
        }
        /* Comentario de linea: se copia hasta el salto de linea. */
        else if (actual == '/' && posicion + 1 < tamano && texto[posicion + 1] == '/') {
            while (posicion < tamano && texto[posicion] != '\n') {
                putchar(texto[posicion++]);
            }
        }
        /* Texto entre comillas o caracter entre apostrofes: se copia completo. */
        else if (actual == '"' || actual == '\'') {
            char cierre = actual;
            putchar(texto[posicion++]);
            while (posicion < tamano && texto[posicion] != cierre) {
                /* Una barra invertida protege al siguiente caracter (\" o \'). */
                if (texto[posicion] == '\\' && posicion + 1 < tamano) {
                    putchar(texto[posicion++]);
                }
                putchar(texto[posicion++]);
            }
            if (posicion < tamano) {
                putchar(texto[posicion++]);
            }
        }
        /* Una palabra empieza con letra o guion bajo. */
        else if (isalpha((unsigned char)actual) || actual == '_') {
            char palabra[64];
            int largo = 0;
            int indice;

            while (posicion < tamano &&
                   (isalnum((unsigned char)texto[posicion]) || texto[posicion] == '_')) {
                if (largo < 63) {
                    palabra[largo++] = texto[posicion];
                }
                posicion++;
            }
            palabra[largo] = '\0';

            indice = buscar_en_tabla(palabra);
            if (indice >= 0) {
                tabla[indice].apariciones++;
                printf("%s", tabla[indice].traduccion);
            } else {
                printf("%s", palabra);
            }
        }
        /* Cualquier otro caracter (espacios, operadores, numeros) pasa igual. */
        else {
            putchar(texto[posicion++]);
        }
    }
}

void imprimir_resumen(void)
{
    int indice;
    int total = 0;

    printf("\n\n===== Palabras reservadas encontradas =====\n");
    for (indice = 0; indice < TOTAL_PALABRAS; indice++) {
        if (tabla[indice].apariciones > 0) {
            printf("%-10s -> %-12s : %d\n", tabla[indice].original,
                   tabla[indice].traduccion, tabla[indice].apariciones);
            total += tabla[indice].apariciones;
        }
    }
    printf("Total de palabras reservadas traducidas: %d\n", total);
}

int main(int argc, char *argv[])
{
    char *programa;
    long tamano = 0;

    if (argc < 2) {
        printf("Uso: %s <archivo.c>\n", argv[0]);
        return 1;
    }

    programa = cargar_archivo(argv[1], &tamano);
    if (programa == NULL) {
        printf("No pude abrir o cargar el archivo %s\n", argv[1]);
        return 1;
    }

    printf("===== Programa traducido (%ld bytes en memoria dinamica) =====\n", tamano);
    traducir_programa(programa, tamano);
    imprimir_resumen();

    free(programa);
    return 0;
}
