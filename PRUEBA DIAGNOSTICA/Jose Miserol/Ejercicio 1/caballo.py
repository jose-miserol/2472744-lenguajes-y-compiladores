"""
Ejercicio 1: validador de movimientos de un caballo de ajedrez.

Dada una casilla inicial y una cadena de movimientos como "Kc2 Kd4 Kb3",
el programa revisa uno por uno si el caballo puede hacerlos. Se detiene
en el primer movimiento incorrecto y dice cual fue.

Un caballo se mueve en "L": dos casillas en una direccion y una en la
otra. Eso significa que la diferencia de columnas y la de filas deben ser
(1, 2) o (2, 1), sin importar el signo.
"""

import os
import sys
import time

COLUMNAS_VALIDAS = "abcdefgh"
FILAS_VALIDAS = "12345678"


def columna_a_numero(letra):
    # 'a' -> 0, 'b' -> 1, ... 'h' -> 7
    return COLUMNAS_VALIDAS.index(letra)


def fila_a_numero(digito):
    # '1' -> 0, '2' -> 1, ... '8' -> 7
    return FILAS_VALIDAS.index(digito)


def es_salto_de_caballo(columna_origen, fila_origen, columna_destino, fila_destino):
    # Cuanto se movio el caballo en cada eje, sin importar hacia donde.
    salto_columnas = abs(columna_destino - columna_origen)
    salto_filas = abs(fila_destino - fila_origen)
    return (salto_columnas == 1 and salto_filas == 2) or (
        salto_columnas == 2 and salto_filas == 1
    )


def verificar_movimientos(posicion_inicial, n, cadena_movimientos):
    """
    Devuelve el texto de salida: CORRECTO, o INCORRECTO con el motivo.
    Solo se revisan los primeros n movimientos de la cadena.
    """
    columna_actual = columna_a_numero(posicion_inicial[0])
    fila_actual = fila_a_numero(posicion_inicial[1])

    movimientos = cadena_movimientos.split()

    numero_movimiento = 1
    while numero_movimiento <= n:
        movimiento = movimientos[numero_movimiento - 1]

        # Un movimiento valido tiene exactamente 3 caracteres: K + columna + fila.
        if len(movimiento) != 3:
            return "movimiento INCORRECTO falla en el movimiento %d" % numero_movimiento

        letra_pieza = movimiento[0]
        letra_columna = movimiento[1]
        digito_fila = movimiento[2]

        # Si la pieza no es K, ni siquiera es un caballo.
        if letra_pieza != "K":
            return "movimiento INCORRECTO falla letra %s no pertenece a un caballo" % letra_pieza

        # La casilla tiene que existir dentro del tablero.
        if letra_columna not in COLUMNAS_VALIDAS or digito_fila not in FILAS_VALIDAS:
            return "movimiento INCORRECTO falla en el movimiento %d" % numero_movimiento

        columna_destino = columna_a_numero(letra_columna)
        fila_destino = fila_a_numero(digito_fila)

        if not es_salto_de_caballo(columna_actual, fila_actual, columna_destino, fila_destino):
            return "movimiento INCORRECTO falla en el movimiento %d" % numero_movimiento

        # El movimiento fue bueno: el caballo queda en la nueva casilla.
        columna_actual = columna_destino
        fila_actual = fila_destino
        numero_movimiento += 1

    return "movimiento CORRECTO"


def ejecutar_pruebas_oficiales():
    """Ejecuta los 9 casos de prueba oficiales del enunciado."""
    print("=== Casos de Prueba Oficiales del Enunciado ===")
    casos = [
        ("a1", 4, "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO"),
        ("a1", 4, "Kc2 Kd4 Kb4 Kc5", "movimiento INCORRECTO falla en el movimiento 3"),
        ("a2", 5, "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO"),
        ("a2", 5, "Kc8 Ke4 Kf6 Ke8 Kg7", "movimiento INCORRECTO falla en el movimiento 1"),
        ("a8", 2, "Kb6 Kd7", "movimiento CORRECTO"),
        ("a8", 2, "Kb6 Kb7", "movimiento INCORRECTO falla en el movimiento 2"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", "movimiento CORRECTO"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4", "movimiento INCORRECTO falla letra Q no pertenece a un caballo"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4", "movimiento INCORRECTO falla en el movimiento 9"),
    ]

    for i, (ak, n, cadena, esperado) in enumerate(casos, 1):
        obtenido = verificar_movimientos(ak, n, cadena)
        estado = "OK" if obtenido == esperado else "FALLO"
        print(f"[{estado}] Prueba {i}: Ak={ak}, n={n}")
        print(f"      Entrada : {cadena}")
        print(f"      Obtenido: {obtenido}")
        print(f"      Esperado: {esperado}\n")


def medir_movimientos(nombre_archivo, repeticiones):
    with open(nombre_archivo, "r", encoding="utf-8") as archivo:
        cadena = archivo.read()
    n = len(cadena.split())

    print("=== Medicion: %d movimientos desde a2, %d repeticiones ===" % (n, repeticiones))

    tiempos = []
    resultado = ""
    for _ in range(repeticiones):
        inicio = time.perf_counter()
        resultado = verificar_movimientos("a2", n, cadena)
        fin = time.perf_counter()
        tiempos.append(fin - inicio)

    for numero, tiempo in enumerate(tiempos, start=1):
        print("Repeticion %2d: %.6f ms" % (numero, tiempo * 1000))

    tiempo_total_ms = sum(tiempos) * 1000
    tiempo_promedio_ms = tiempo_total_ms / repeticiones

    print("Resultado: %s" % resultado)
    print("Tiempo total de las %d repeticiones: %.6f ms" % (repeticiones, tiempo_total_ms))
    print("Tiempo promedio por repeticion:      %.6f ms" % tiempo_promedio_ms)

    # Guardar reporte en archivo txt
    ruta_salida = os.path.join(os.path.dirname(nombre_archivo), "tiempos_ejercicio1.txt")
    with open(ruta_salida, "w", encoding="utf-8") as f:
        f.write("===================================================\n")
        f.write("RESULTADOS EJERCICIO 1: AJEDREZ (PYTHON)\n")
        f.write("Estudiante: Jose Miserol\n")
        f.write("===================================================\n")
        f.write("Movimientos evaluados: %d\n" % n)
        f.write("Posicion inicial: a2\n")
        f.write("Estado: %s\n" % resultado)
        f.write("Tiempo total (10 iteraciones): %.6f ms\n" % tiempo_total_ms)
        f.write("Tiempo promedio por corrida: %.6f ms\n" % tiempo_promedio_ms)
        f.write("---------------------------------------------------\n")
    print(f"\n-> Reporte guardado con exito en '{os.path.basename(ruta_salida)}'")


if __name__ == "__main__":
    directorio_actual = os.path.dirname(os.path.abspath(__file__))
    ruta_defecto = os.path.join(directorio_actual, "movimientos.txt")
    if len(sys.argv) > 1:
        archivo_movimientos = sys.argv[1]
        if not os.path.exists(archivo_movimientos):
            archivo_movimientos = os.path.join(directorio_actual, sys.argv[1])
    else:
        archivo_movimientos = ruta_defecto

    ejecutar_pruebas_oficiales()
    medir_movimientos(archivo_movimientos, 10)
