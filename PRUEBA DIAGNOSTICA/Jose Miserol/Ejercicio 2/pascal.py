"""
Ejercicio 2 (version Python): coeficientes de (x+1)^n con el triangulo de Pascal.

Uso: python3 pascal.py <n> <x> [repeticiones]

Hace tres cosas:
  a) genera los coeficientes de (x+1)^n y muestra el polinomio,
  b) evalua f(x) = (x+1)^n paso a paso usando esos coeficientes,
  c) mide cuanto tarda en generar el triangulo para n dado.

Memoria dinamica: el triangulo es una lista de listas. Cada fila se
crea con el tamano que necesita (fila i tiene i+1 numeros) y se agrega
al triangulo cuando esta lista.
"""

import sys
import time


def generar_triangulo(n):
    """Devuelve las filas 0..n del triangulo de Pascal."""
    triangulo = []

    for numero_fila in range(n + 1):
        fila = []
        for posicion in range(numero_fila + 1):
            # Los bordes de cada fila siempre valen 1.
            if posicion == 0 or posicion == numero_fila:
                fila.append(1)
            else:
                # Cada numero interior es la suma de los dos que tiene encima.
                fila_anterior = triangulo[numero_fila - 1]
                fila.append(fila_anterior[posicion - 1] + fila_anterior[posicion])
        triangulo.append(fila)

    return triangulo


def texto_del_polinomio(coeficientes):
    """Arma el texto tipo x^3+3x^2+3x+1 a partir de la ultima fila."""
    grado = len(coeficientes) - 1
    terminos = []

    for posicion, coeficiente in enumerate(coeficientes):
        potencia = grado - posicion

        # Un coeficiente 1 no se escribe delante de la x (x^2 y no 1x^2).
        if coeficiente == 1 and potencia > 0:
            parte_numerica = ""
        else:
            parte_numerica = str(coeficiente)

        if potencia == 0:
            terminos.append(parte_numerica)
        elif potencia == 1:
            terminos.append(parte_numerica + "x")
        else:
            terminos.append(parte_numerica + "x^" + str(potencia))

    return "+".join(terminos)


def evaluar_por_pasos(coeficientes, x):
    """Calcula f(x) mostrando cada termino coeficiente * x^potencia."""
    grado = len(coeficientes) - 1
    suma = 0

    for posicion, coeficiente in enumerate(coeficientes):
        potencia = grado - posicion
        valor_termino = coeficiente * (x ** potencia)
        suma += valor_termino
        print("  paso %d: %d * %d^%d = %d   (suma parcial = %d)"
              % (posicion + 1, coeficiente, x, potencia, valor_termino, suma))

    return suma


def medir_generacion(n, repeticiones):
    """Promedia el tiempo de generar el triangulo hasta n."""
    inicio = time.perf_counter()
    for _ in range(repeticiones):
        generar_triangulo(n)
    fin = time.perf_counter()
    return (fin - inicio) / repeticiones


if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 5
    x = int(sys.argv[2]) if len(sys.argv) > 2 else 2
    repeticiones = int(sys.argv[3]) if len(sys.argv) > 3 else 1000

    triangulo = generar_triangulo(n)
    coeficientes = triangulo[n]

    print("n = %d" % n)
    print("Coeficientes: %s" % " ".join(str(c) for c in coeficientes))
    print("(x+1)^%d = %s" % (n, texto_del_polinomio(coeficientes)))

    print("\nCalculo de f(%d) = (x+1)^%d por pasos:" % (x, n))
    resultado = evaluar_por_pasos(coeficientes, x)
    print("f(%d) = %d" % (x, resultado))
    print("Comprobacion con (x+1)**n directo: %d" % ((x + 1) ** n))

    promedio = medir_generacion(n, repeticiones)
    print("\nTiempo promedio para generar n=%d (%d repeticiones): %.3f microsegundos"
          % (n, repeticiones, promedio * 1e6))
