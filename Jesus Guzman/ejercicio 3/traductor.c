#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ============================================================================
// ESTRUCTURA: KeywordMap
// Asocia una palabra reservada en C con su traduccion en idioma espanol.
// ============================================================================
typedef struct {
    const char* c_kw;
    const char* es_kw;
} KeywordMap;

// Tabla exhaustiva de palabras clave del lenguaje C
static const KeywordMap TABLA[] = {
    {"int", "entero"}, {"float", "flotante"}, {"double", "doble"},
    {"char", "caracter"}, {"void", "vacio"}, {"long", "largo"},
    {"short", "corto"}, {"unsigned", "sin_signo"}, {"signed", "con_signo"},
    {"if", "si"}, {"else", "sino"}, {"while", "mientras"},
    {"for", "para"}, {"do", "hacer"}, {"return", "retornar"},
    {"switch", "cambiar"}, {"case", "caso"}, {"break", "romper"},
    {"default", "defecto"}, {"continue", "continuar"}, {"struct", "estructura"},
    {"typedef", "definir_tipo"}, {"sizeof", "tamano_de"}, {"const", "constante"},
    {"static", "estatico"}, {"enum", "enumeracion"}, {"union", "union"},
    {"goto", "ir_a"}, {"extern", "externo"}, {"auto", "automatico"},
    {"register", "registro"}, {"volatile", "volatil"},
    {NULL, NULL}
};

const char* buscar_traduccion(const char* id) {
    for (int i = 0; TABLA[i].c_kw != NULL; i++) {
        if (strcmp(TABLA[i].c_kw, id) == 0) {
            return TABLA[i].es_kw;
        }
    }
    return NULL;
}

// ============================================================================
// FUNCION: cargar_en_memoria
// Carga el archivo completo en memoria dinamica (RAM) con malloc.
// ============================================================================
char* cargar_en_memoria(const char* ruta, long* tamano) {
    FILE* f = fopen(ruta, "rb");
    if (!f) return NULL;

    // Calculo del tamano exacto del archivo con fseek y ftell
    fseek(f, 0, SEEK_END);
    *tamano = ftell(f);
    rewind(f);

    // Asignacion de memoria dinamica para el buffer de texto
    char* buffer = (char*)malloc(*tamano + 1);
    if (!buffer) { fclose(f); return NULL; }

    fread(buffer, 1, *tamano, f);
    buffer[*tamano] = '\0';
    fclose(f);
    return buffer;
}

// ============================================================================
// FUNCION: traducir_codigo
// Analizador lexico que filtra comentarios, cadenas y traduce palabras clave.
// ============================================================================
int traducir_codigo(const char* src, const char* out_ruta) {
    FILE* out = fopen(out_ruta, "w");
    if (!out) return 0;

    int palabras_traducidas = 0;
    int i = 0;

    while (src[i] != '\0') {
        // 1. FILTRO: Preservar cadenas de texto literales "..."
        if (src[i] == '"') {
            fputc(src[i++], out);
            while (src[i] != '\0' && src[i] != '"') {
                if (src[i] == '\\' && src[i + 1] != '\0') {
                    fputc(src[i++], out);
                }
                fputc(src[i++], out);
            }
            if (src[i] == '"') fputc(src[i++], out);
            continue;
        }

        // 2. FILTRO: Preservar caracteres literales '...'
        if (src[i] == '\'') {
            fputc(src[i++], out);
            while (src[i] != '\0' && src[i] != '\'') {
                if (src[i] == '\\' && src[i + 1] != '\0') {
                    fputc(src[i++], out);
                }
                fputc(src[i++], out);
            }
            if (src[i] == '\'') fputc(src[i++], out);
            continue;
        }

        // 3. FILTRO: Preservar comentarios de una linea //
        if (src[i] == '/' && src[i + 1] == '/') {
            while (src[i] != '\0' && src[i] != '\n') {
                fputc(src[i++], out);
            }
            continue;
        }

        // 4. FILTRO: Preservar comentarios multilineas /* ... */
        if (src[i] == '/' && src[i + 1] == '*') {
            fputc(src[i++], out);
            fputc(src[i++], out);
            while (src[i] != '\0' && !(src[i] == '*' && src[i + 1] == '/')) {
                fputc(src[i++], out);
            }
            if (src[i] != '\0') { fputc(src[i++], out); fputc(src[i++], out); }
            continue;
        }

        // 5. EXTRACCION LEXICA: Palabras reservadas e identificadores
        if (isalpha(src[i]) || src[i] == '_') {
            char token[128];
            int k = 0;
            while (isalnum(src[i]) || src[i] == '_') {
                if (k < 127) token[k++] = src[i];
                i++;
            }
            token[k] = '\0';

            const char* traduccion = buscar_traduccion(token);
            if (traduccion) {
                fputs(traduccion, out);
                palabras_traducidas++;
            } else {
                fputs(token, out);
            }
            continue;
        }

        // Simbolos de puntuacion, operadores, llaves y espacios en blanco
        fputc(src[i++], out);
    }

    fclose(out);
    return palabras_traducidas;
}

int main(int argc, char* argv[]) {
    printf("===================================================================\n");
    printf("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)\n");
    printf("  CATEDRA: LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA\n");
    printf("  EJERCICIO 3: TRADUCTOR LEXICO EN C CON MEMORIA DINAMICA\n");
    printf("  Estudiante: Jesus Guzman\n");
    printf("===================================================================\n\n");

    const char* ruta_in = (argc > 1) ? argv[1] : "entrada.c";
    const char* ruta_out = (argc > 2) ? argv[2] : "salida_espanol.c";

    long tamano = 0;
    clock_t start = clock();
    char* memoria = cargar_en_memoria(ruta_in, &tamano);
    if (!memoria) {
        printf("Error: no se pudo cargar '%s' en memoria dinamica.\n", ruta_in);
        return 1;
    }

    int palabras = traducir_codigo(memoria, ruta_out);
    clock_t end = clock();
    double tiempo_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("Archivo '%s' (%ld bytes) cargado en MEMORIA DINAMICA (RAM).\n", ruta_in, tamano);
    printf("Palabras reservadas traducidas : %d\n", palabras);
    printf("Tiempo de traduccion           : %.4f ms\n", tiempo_ms);
    printf("Archivo traducido guardado en  : '%s'\n\n", ruta_out);

    // Muestra del contenido traducido en consola
    printf("--- CONTENIDO GENERADO ('%s') ---\n", ruta_out);
    FILE* f_out = fopen(ruta_out, "r");
    if (f_out) {
        char linea[256];
        while (fgets(linea, sizeof(linea), f_out)) {
            printf("%s", linea);
        }
        fclose(f_out);
    }
    printf("\n-------------------------------------------------------------------\n\n");

    // Guardado de tiempos en txt
    FILE* f = fopen("tiempos_ejercicio3.txt", "w");
    if (f) {
        fprintf(f, "===================================================\n");
        fprintf(f, "RESULTADOS EJERCICIO 3: TRADUCTOR LEXICO C\n");
        fprintf(f, "Estudiante: Jesus Guzman\n");
        fprintf(f, "===================================================\n");
        fprintf(f, "[C] Archivo procesado: %s (%ld bytes)\n", ruta_in, tamano);
        fprintf(f, "[C] Palabras reservadas traducidas: %d\n", palabras);
        fprintf(f, "[C] Tiempo de ejecucion: %.4f ms\n", tiempo_ms);
        fprintf(f, "---------------------------------------------------\n");
        fclose(f);
    }

    free(memoria); // Liberacion de memoria dinamica asignada con malloc
    return 0;
}
