#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *ingles;
    const char *espanol;
} PalabraReservada;

PalabraReservada diccionario[] = {
    {"auto", "automatico"},
    {"break", "romper / interrumpir"},
    {"case", "caso"},
    {"char", "caracter"},
    {"const", "constante"},
    {"continue", "continuar"},
    {"default", "por_defecto"},
    {"do", "hacer"},
    {"double", "doble"},
    {"else", "sino"},
    {"enum", "enumeracion"},
    {"extern", "externo"},
    {"float", "flotante"},
    {"for", "para"},
    {"goto", "ir_a"},
    {"if", "si"},
    {"int", "entero"},
    {"long", "largo"},
    {"register", "registro"},
    {"return", "retornar"},
    {"short", "corto"},
    {"signed", "con_signo"},
    {"sizeof", "tamano_de"},
    {"static", "estatico"},
    {"struct", "estructura"},
    {"switch", "segun"},
    {"typedef", "definir_tipo"},
    {"union", "union"},
    {"unsigned", "sin_signo"},
    {"void", "vacio"},
    {"volatile", "volatil"},
    {"while", "mientras"}
};

int total_palabras = sizeof(diccionario) / sizeof(diccionario[0]);

char* buscar_traduccion(const char *palabra) {
    for (int i = 0; i < total_palabras; i++) {
        if (strcmp(diccionario[i].ingles, palabra) == 0) {
            return (char*)diccionario[i].espanol;
        }
    }
    return NULL;
}

char* cargar_archivo_dinamico(const char *nombre_archivo, long *tamano_out) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) return NULL;

    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *buffer = (char*) malloc(tamano + 1);
    if (!buffer) {
        fclose(archivo);
        return NULL;
    }

    long leidos = fread(buffer, 1, tamano, archivo);
    buffer[leidos] = '\0';
    fclose(archivo);

    *tamano_out = leidos;
    return buffer;
}

void analizar_y_traducir(const char *contenido, const char *nombre_archivo_salida) {
    char *copia = _strdup(contenido);
    if (!copia) return;

    FILE *f_salida = fopen(nombre_archivo_salida, "w");
    if (f_salida) {
        fprintf(f_salida, "=====================================================\n");
        fprintf(f_salida, "REPORTE DE TRADUCCION DE PALABRAS RESERVADAS EN C\n");
        fprintf(f_salida, "Estudiante: Franklyn Quintero (Ejercicio 3)\n");
        fprintf(f_salida, "=====================================================\n\n");
    }

    printf("--- PALABRAS RESERVADAS ENCONTRADAS Y TRADUCIDAS ---\n");
    
    const char *delimitadores = " \t\n(),;{}[]=+-*/<>#\"'";
    char *token = strtok(copia, delimitadores);
    int encontradas = 0;

    while (token != NULL) {
        char *traduccion = buscar_traduccion(token);
        if (traduccion != NULL) {
            printf("[EN] %-10s  -->  [ES] %s\n", token, traduccion);
            if (f_salida) {
                fprintf(f_salida, "[EN] %-10s  -->  [ES] %s\n", token, traduccion);
            }
            encontradas++;
        }
        token = strtok(NULL, delimitadores);
    }

    printf("\nTotal de palabras reservadas detectadas: %d\n", encontradas);
    if (f_salida) {
        fprintf(f_salida, "\nTotal de palabras reservadas detectadas: %d\n", encontradas);
        fclose(f_salida);
        printf("Reporte guardado con exito en: %s\n", nombre_archivo_salida);
    }

    free(copia);
}

int main(int argc, char *argv[]) {
    const char *nombre_archivo = (argc > 1) ? argv[1] : "main.c";
    const char *nombre_salida = "salida_diccionario.txt";
    long tamano = 0;

    char *memoria_dinamica = cargar_archivo_dinamico(nombre_archivo, &tamano);
    if (!memoria_dinamica) {
        printf("Error: No se pudo cargar el archivo '%s' en memoria dinamica.\n", nombre_archivo);
        return 1;
    }

    printf("--- EJERCICIO 3: TRADUCTOR DE PALABRAS RESERVADAS DE C ---\n");
    printf("Codigo fuente cargado dinamicamente en memoria desde '%s' (%ld bytes).\n\n", nombre_archivo, tamano);
    
    analizar_y_traducir(memoria_dinamica, nombre_salida);

    free(memoria_dinamica);
    return 0;
}

