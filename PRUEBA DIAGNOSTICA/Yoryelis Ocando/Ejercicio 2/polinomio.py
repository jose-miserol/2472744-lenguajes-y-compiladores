"""
============================================================
Ejercicio 2: Coeficientes de (x+1)^n con el triangulo de Pascal
Lenguaje: Python
============================================================
"""

import os
import sys
import time

# Veces que se repite la generacion al medir el tiempo.
REPETICIONES = 1000
N_BENCHMARK = 100

# Filas 0 a 7 del triangulo de Pascal
TABLA_ENUNCIADO = [
    [1],
    [1, 1],
    [1, 2, 1],
    [1, 3, 3, 1],
    [1, 4, 6, 4, 1],
    [1, 5, 10, 10, 5, 1],
    [1, 6, 15, 20, 15, 6, 1],
    [1, 7, 21, 35, 35, 21, 7, 1],
]

# Casos de evaluacion: se calcula (x+1)^n para cada pareja (n, x).
CASOS = [(0, 3), (1, 2), (2, 5), (5, 2), (7, 3), (10, 4)]


def generar_fila(n):
    """
    Genera la fila n del triangulo de Pascal usando memoria
    dinamica (una lista de Python).

    Cada fila se construye a partir de la anterior recorriendo
    de derecha a izquierda para no perder datos.
    """
    fila = [1]

    for r in range(1, n + 1):
        fila.append(1)

        for j in range(r - 1, 0, -1):
            fila[j] = fila[j] + fila[j - 1]

    return fila


def generar_triangulo(n):
    """
    Genera todas las filas del triangulo de Pascal desde 0 hasta n.
    Devuelve una lista de listas.
    """
    triangulo = []

    for r in range(n + 1):
        fila = [1] * (r + 1)

        for j in range(1, r):
            fila[j] = triangulo[r - 1][j - 1] + triangulo[r - 1][j]

        triangulo.append(fila)

    return triangulo


def texto_polinomio(coeficientes):
    """Devuelve el polinomio (x+1)^n como texto, de mayor a menor potencia."""
    n = len(coeficientes) - 1
    terminos = []

    for k, coeficiente in enumerate(coeficientes):
        potencia = n - k

        if potencia == 0:
            termino = str(coeficiente)
        elif potencia == 1:
            termino = "x" if coeficiente == 1 else f"{coeficiente}x"
        elif coeficiente == 1:
            termino = f"x^{potencia}"
        else:
            termino = f"{coeficiente}x^{potencia}"

        terminos.append(termino)

    return f"(x+1)^{n} = " + " + ".join(terminos)


def probar_tabla_enunciado():
    """Prueba 1: las filas generadas deben coincidir con la tabla del enunciado."""
    print("========================================")
    print("  Prueba 1 - Filas del enunciado (n = 0..7)")
    print("========================================")

    aprobadas = 0

    for n, esperada in enumerate(TABLA_ENUNCIADO):
        coincide = generar_fila(n) == esperada
        print(f"Fila n = {n}: {'CORRECTA' if coincide else 'INCORRECTA'}")
        if coincide:
            aprobadas += 1

    print(f"Filas correctas: {aprobadas} de {len(TABLA_ENUNCIADO)}")
    return aprobadas == len(TABLA_ENUNCIADO)


def probar_evaluacion(n, x):
    """
    Prueba 2: genera el polinomio, evalua f(x) por pasos y compara
    con (x+1)^n calculado directamente.
    """
    coeficientes = generar_fila(n)

    # Potencias x^0, x^1, ..., x^n.
    potencias = [1] * (n + 1)
    for i in range(1, n + 1):
        potencias[i] = potencias[i - 1] * x

    print("\n----------------------------------------")
    print(f"n = {n}, x = {x}")
    print("a) Coeficientes: " + " ".join(str(c) for c in coeficientes))
    print("   Polinomio: " + texto_polinomio(coeficientes))
    print("b) Calculo por pasos:")

    suma = 0
    for k, coeficiente in enumerate(coeficientes):
        potencia = n - k
        termino = coeficiente * potencias[potencia]
        suma += termino
        print(
            f"   Paso {k + 1}: {coeficiente} * ({x})^{potencia} "
            f"= {termino} | acumulado = {suma}"
        )

    directo = (x + 1) ** n
    print(f"   f({x}) segun el polinomio = {suma}")
    print(f"   (x+1)^n calculado directo = {directo}")

    correcto = suma == directo
    print(f"   Resultado: {'CORRECTO' if correcto else 'INCORRECTO'}")
    return correcto


def medir_tiempo():
    """
    Prueba 3: mide el tiempo de generar los polinomios (x+1)^n para
    n = 0..100. El resultado se muestra en pantalla y se guarda en
    resultados_ejercicio2_python.txt.
    """
    n = N_BENCHMARK

    inicio = time.perf_counter_ns()
    triangulo = generar_triangulo(n)
    una = (time.perf_counter_ns() - inicio) / 1000.0  # microsegundos
    central = triangulo[n][n // 2]

    inicio = time.perf_counter_ns()
    for _ in range(REPETICIONES):
        generar_triangulo(n)
    total = (time.perf_counter_ns() - inicio) / 1000.0
    promedio = total / REPETICIONES

    lineas = [
        "========================================",
        "  Ejercicio 2 - Triangulo de Pascal (Python)",
        "========================================",
        f"Polinomios generados      : (x+1)^n, n = 0..{n} ({n + 1} polinomios)",
        f"Coeficiente central n={n} : {central}",
        "----------------------------------------",
        f"1 generacion              : {una:10.3f} microsegundos ({una / 1000.0:.6f} milisegundos)",
        f"Promedio de {REPETICIONES} generaciones : {promedio:10.3f} microsegundos "
        f"({promedio / 1000.0:.6f} milisegundos)",
        "========================================",
    ]

    texto = "\n".join(lineas)
    print()
    print(texto)

    carpeta = os.path.dirname(os.path.abspath(__file__))
    ruta = os.path.join(carpeta, "resultados_ejercicio2_python.txt")

    with open(ruta, "w", encoding="utf-8") as archivo:
        archivo.write(texto + "\n")

    print(f"\nResultado guardado en {ruta}")


def main():
    todo_correcto = probar_tabla_enunciado()

    print("\n========================================")
    print("  Prueba 2 - Evaluacion de f(x) = (x+1)^n")
    print("========================================", end="")

    aprobados = sum(1 for n, x in CASOS if probar_evaluacion(n, x))
    print(f"\nEvaluaciones correctas: {aprobados} de {len(CASOS)}")

    if aprobados != len(CASOS):
        todo_correcto = False

    medir_tiempo()

    return 0 if todo_correcto else 1


if __name__ == "__main__":
    sys.exit(main())