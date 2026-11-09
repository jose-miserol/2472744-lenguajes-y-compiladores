/*
 * ============================================================
 * Ejercicio 3 (C): cargar un programa C en memoria dinamica,
 * detectar las palabras reservadas y traducirlas al espanol.
 *
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Estructura reservada */
typedef struct {
    const char *en;   /* palabra reservada en ingles */
    const char *es;   /* palabra reservada en espanol */
    int veces;        /* numero de veces que aparece */
    int cap;          /* capacidad actual del arreglo de lineas */
    int *lineas;      /* lineas donde aparece (memoria dinamica) */
} Reservada;

#define R(en, es) {en, es, 0, 0, NULL}

static Reservada tabla[] = {
    R("auto", "automatico"), R("break", "romper"), R("case", "caso"), R("char", "caracter"),
    R("const", "constante"), R("continue", "continuar"), R("default", "por_defecto"),
    R("do", "hacer"), R("double", "doble"), R("else", "sino"), R("enum", "enumeracion"),
    R("extern", "externo"), R("float", "flotante"), R("for", "para"), R("goto", "ir_a"),
    R("if", "si"), R("int", "entero"), R("long", "largo"), R("register", "registro"),
    R("return", "retornar"), R("short", "corto"), R("signed", "con_signo"),
    R("sizeof", "tamano_de"), R("static", "estatico"), R("struct", "estructura"),
    R("switch", "segun"), R("typedef", "definir_tipo"), R("union", "union"),
    R("unsigned", "sin_signo"), R("void", "vacio"), R("volatile", "volatil"),
    R("while", "mientras"),
    /* C99 */
    R("inline", "en_linea"), R("restrict", "restringido"), R("_Bool", "booleano"),
    R("_Complex", "complejo"), R("_Imaginary", "imaginario"),
    /* C11 */
    R("_Alignas", "alinear_como"), R("_Alignof", "alineacion_de"), R("_Atomic", "atomico"),
    R("_Generic", "generico"), R("_Noreturn", "sin_retorno"),
    R("_Static_assert", "asercion_estatica"), R("_Thread_local", "local_al_hilo")
};
#define N_RES ((int)(sizeof tabla / sizeof tabla[0]))

/*
 *  Las palabras que estan dentro de comentarios
 * o de cadenas de texto no deben contarse ni traducirse.
 */
static const char *PROGRAMA_PRUEBA =
    "#include <stdio.h>\n"
    "/* if while dentro de un comentario no se traduce */\n"
    "int main(void) {\n"
    "    int suma = 0;\n"
    "    for (int i = 0; i < 5; i++) {\n"
    "        if (i == 3) continue;\n"
    "        suma += i;\n"
    "    }\n"
    "    while (suma > 0) {\n"
    "        suma--;\n"
    "    }\n"
    "    printf(\"return if for dentro de una cadena\\n\");\n"
    "    return 0;\n"
    "}\n";

/* Cantidades que deben encontrarse en el programa de prueba. */
typedef struct {
    const char *palabra;
    int esperado;
} Esperado;

static const Esperado ESPERADOS[] = {
    {"int", 3}, {"void", 1}, {"for", 1}, {"if", 1},
    {"continue", 1}, {"while", 1}, {"return", 1}
};
#define N_ESPERADOS ((int)(sizeof ESPERADOS / sizeof ESPERADOS[0]))

static Reservada *buscar(const char *palabra) {
    for (int i = 0; i < N_RES; i++)
        if (strcmp(tabla[i].en, palabra) == 0) return &tabla[i];
    return NULL;
}

static int registrar(Reservada *r, int linea) {
    if (r->veces == r->cap) {
        int nueva = r->cap ? r->cap * 2 : 4;
        int *tmp = realloc(r->lineas, nueva * sizeof(int));
        if (!tmp) return 0;
        r->lineas = tmp;
        r->cap = nueva;
    }
    r->lineas[r->veces++] = linea;
    return 1;
}

/* Copia un texto a memoria dinamica. */
static char *copiar_a_memoria(const char *texto, long *tam) {
    *tam = (long)strlen(texto);
    char *buf = malloc(*tam + 1);
    if (buf) memcpy(buf, texto, *tam + 1);
    return buf;
}

/* Carga un archivo completo en memoria dinamica. */
static char *cargar(const char *ruta, long *tam) {
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    *tam = ftell(f);
    rewind(f);
    char *buf = malloc(*tam + 1);
    if (buf) {
        size_t n = fread(buf, 1, *tam, f);
        buf[n] = '\0';
        *tam = (long)n;
    }
    fclose(f);
    return buf;
}

/* Recorre el codigo, cuenta palabras reservadas y escribe la version traducida. */
static void analizar(const char *s, long tam, FILE *traducido) {
    long i = 0;
    int linea = 1, inicio_linea = 1;

    while (i < tam) {
        char c = s[i];

        /* lineas #include: se copian tal cual (por ejemplo <float.h>) */
        if (inicio_linea && c == '#') {
            long j = i + 1;
            while (j < tam && (s[j] == ' ' || s[j] == '\t')) j++;
            if (strncmp(s + j, "include", 7) == 0) {
                while (i < tam && s[i] != '\n') fputc(s[i++], traducido);
                continue;
            }
        }
        if (c == '\n') { linea++; inicio_linea = 1; fputc(c, traducido); i++; continue; }
        if (c != ' ' && c != '\t' && c != '\r') inicio_linea = 0;

        /* comentario de linea */
        if (c == '/' && i + 1 < tam && s[i + 1] == '/') {
            while (i < tam && s[i] != '\n') fputc(s[i++], traducido);
            continue;
        }
        /* comentario de bloque */
        if (c == '/' && i + 1 < tam && s[i + 1] == '*') {
            fputc(s[i++], traducido); fputc(s[i++], traducido);
            while (i < tam && !(s[i] == '*' && i + 1 < tam && s[i + 1] == '/')) {
                if (s[i] == '\n') linea++;
                fputc(s[i++], traducido);
            }
            if (i < tam) { fputc(s[i++], traducido); fputc(s[i++], traducido); }
            continue;
        }
        /* cadena o caracter */
        if (c == '"' || c == '\'') {
            char q = c;
            fputc(s[i++], traducido);
            while (i < tam && s[i] != q && s[i] != '\n') {
                if (s[i] == '\\' && i + 1 < tam) fputc(s[i++], traducido);
                fputc(s[i++], traducido);
            }
            if (i < tam && s[i] == q) fputc(s[i++], traducido);
            continue;
        }
        /* identificador / palabra */
        if (isalpha((unsigned char)c) || c == '_') {
            long ini = i;
            while (i < tam && (isalnum((unsigned char)s[i]) || s[i] == '_')) i++;
            long len = i - ini;
            char palabra[64];
            if (len < (long)sizeof palabra) {
                memcpy(palabra, s + ini, len);
                palabra[len] = '\0';
                Reservada *r = buscar(palabra);
                if (r) {
                    registrar(r, linea);
                    fputs(r->es, traducido);
                    continue;
                }
            }
            fwrite(s + ini, 1, len, traducido);
            continue;
        }
        fputc(c, traducido);
        i++;
    }
}

static void reporte(FILE *f, const char *origen) {
    int distintas = 0, total = 0;
    fprintf(f, "Programa analizado: %s\n\n", origen);
    fprintf(f, "%-16s %-20s %-7s %s\n", "Palabra (C)", "Espanol", "Veces", "Lineas");
    fprintf(f, "------------------------------------------------------------------\n");
    for (int i = 0; i < N_RES; i++) {
        Reservada *r = &tabla[i];
        if (r->veces == 0) continue;
        distintas++;
        total += r->veces;
        fprintf(f, "%-16s %-20s %-7d ", r->en, r->es, r->veces);
        for (int k = 0; k < r->veces; k++) fprintf(f, "%s%d", k ? "," : "", r->lineas[k]);
        fprintf(f, "\n");
    }
    fprintf(f, "\nPalabras reservadas distintas: %d | Apariciones totales: %d\n", distintas, total);
}

/* Muestra un archivo de texto en pantalla. */
static void mostrar_archivo(const char *ruta) {
    FILE *f = fopen(ruta, "r");
    int c;
    if (!f) return;
    while ((c = fgetc(f)) != EOF) putchar(c);
    fclose(f);
}

/* Compara las cantidades encontradas con las esperadas. Devuelve 1 si todas coinciden. */
static int comprobar_esperados(void) {
    int aprobadas = 0;

    printf("\n========================================\n");
    printf("  Comprobacion de cantidades esperadas\n");
    printf("========================================\n");

    for (int i = 0; i < N_ESPERADOS; i++) {
        Reservada *r = buscar(ESPERADOS[i].palabra);
        int obtenido = r ? r->veces : 0;
        int coincide = (obtenido == ESPERADOS[i].esperado);
        if (coincide) aprobadas++;
        printf("%-10s esperado = %d | obtenido = %d | %s\n",
               ESPERADOS[i].palabra, ESPERADOS[i].esperado, obtenido,
               coincide ? "CORRECTO" : "INCORRECTO");
    }

    printf("Comprobaciones correctas: %d de %d\n", aprobadas, N_ESPERADOS);
    return aprobadas == N_ESPERADOS;
}

int main(int argc, char *argv[]) {
    const char *origen;
    long tam = 0;
    char *codigo;
    int correcto = 1;

    if (argc > 1) {
        origen = argv[1];
        codigo = cargar(origen, &tam);
        if (!codigo) { printf("No se pudo abrir %s\n", origen); return 1; }
    } else {
        origen = "programa de prueba incluido";
        codigo = copiar_a_memoria(PROGRAMA_PRUEBA, &tam);
        if (!codigo) { printf("Error de memoria.\n"); return 1; }
    }

    printf("========================================\n");
    printf("  Ejercicio 3 - Palabras reservadas (C)\n");
    printf("========================================\n");
    printf("Programa cargado en memoria dinamica: %ld caracteres\n", tam);
    printf("\n--- Programa original ---\n%s\n", codigo);

    FILE *trad = fopen("programa_traducido.txt", "w");
    if (!trad) { free(codigo); printf("No se pudo crear programa_traducido.txt\n"); return 1; }
    analizar(codigo, tam, trad);
    fclose(trad);

    printf("--- Programa traducido ---\n");
    mostrar_archivo("programa_traducido.txt");

    printf("\n--- Reporte ---\n");
    reporte(stdout, origen);
    FILE *rep = fopen("reporte_palabras_reservadas.txt", "w");
    if (rep) { reporte(rep, origen); fclose(rep); }

    if (argc <= 1) correcto = comprobar_esperados();

    printf("\nGenerados: reporte_palabras_reservadas.txt y programa_traducido.txt\n");

    for (int i = 0; i < N_RES; i++) free(tabla[i].lineas);
    free(codigo);
    return correcto ? 0 : 1;
}