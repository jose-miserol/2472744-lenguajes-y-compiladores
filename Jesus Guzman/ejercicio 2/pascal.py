
import time
import math

# ============================================================================
# FUNCION: generar_pascal
# Genera la fila 'n' del Triangulo de Pascal usando listas dinamicas en RAM.
# ============================================================================
def generar_pascal(n: int) -> list[int]:
    """
    Construye iterativamente los coeficientes binomiales de (x+1)^n.
    En Python, las listas son dinamicas y los enteros tienen precision arbitraria,
    lo que evita problemas de desbordamiento numerico para n = 100.
    """
    fila = [1]
    for _ in range(n):
        fila = [1] + [fila[i] + fila[i + 1] for i in range(len(fila) - 1)] + [1]
    return fila

# ============================================================================
# FUNCION: mostrar_polinomio
# Construye la representacion en texto algebraico del polinomio expandido.
# ============================================================================
def mostrar_polinomio(coef: list[int]) -> str:
    n = len(coef) - 1
    terminos = []
    for i, c in enumerate(coef):
        pot = n - i
        if pot == 0:
            terminos.append(f"{c}")
        elif pot == 1:
            terminos.append(f"{c}x" if c != 1 else "x")
        else:
            terminos.append(f"{c}x^{pot}" if c != 1 else f"x^{pot}")
    return " + ".join(terminos)

# ============================================================================
# FUNCION: evaluar_paso_a_paso
# Calcula f(x) = (x+1)^n mostrando la acumulacion termino por termino.
# ============================================================================
def evaluar_paso_a_paso(coef: list[int], x: float):
    n = len(coef) - 1
    print(f"\n--- Calculo paso a paso para x = {x:.2f} en (x + 1)^{n} ---")
    acumulado = 0.0
    for i, c in enumerate(coef):
        pot = n - i
        x_pot = x ** pot
        termino = c * x_pot
        acumulado += termino
        print(f"Paso {i+1:2d}: Coef = {c} | x^{pot} = {x_pot:.4f} | Termino = {termino:.4f} | Acumulado = {acumulado:.4f}")
    
    print(f"Resultado final f({x:.2f}) = {acumulado:.4f}")
    print(f"Comprobacion directa ({x:.2f} + 1)^{n} = {(x + 1.0)**n:.4f}\n")

def main():
    print("===================================================================")
    print("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)")
    print("  CATEDRA: LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA")
    print("  EJERCICIO 2: POLINOMIO (x+1)^n Y PASCAL CON LISTAS DINAMICAS (PYTHON)")
    print("  Estudiante: Jesus Guzman")
    print("===================================================================\n")

    # 1. Mostrar primeros polinomios como en la tabla del enunciado
    print("--- EJEMPLOS BASE DEL TRIANGULO DE PASCAL (n = 0 hasta 5) ---")
    for k in range(6):
        c = generar_pascal(k)
        print(f"(x + 1)^{k} = {mostrar_polinomio(c)}")

    # 2. Evaluacion paso a paso para n = 4 con x = 2.0
    n_demo = 4
    x_demo = 2.0
    coef_demo = generar_pascal(n_demo)
    evaluar_paso_a_paso(coef_demo, x_demo)

    # 3. Demostracion para n = 100 y coeficiente central
    print("--- DEMOSTRACION PARA n = 100 (101 ELEMENTOS EN LISTA DINAMICA) ---")
    c100_demo = generar_pascal(100)
    print(f"Coeficiente central (posicion 50) de (x+1)^100: {c100_demo[50]}")
    print("(Se comprueba la precision exacta de 30 digitos sin desbordamiento)\n")

    # 4. Medicion de rendimiento de alta resolucion para n = 100 con 2,000 repeticiones
    repeticiones = 2000
    print(f"--- MEDICION DE TIEMPO EN PYTHON (n = 100 x {repeticiones} ITERACIONES) ---")
    inicio = time.perf_counter()
    for _ in range(repeticiones):
        _ = generar_pascal(100)
    fin = time.perf_counter()

    tiempo_total_ms = (fin - inicio) * 1000.0
    tiempo_promedio_ms = tiempo_total_ms / repeticiones

    print(f"Tiempo total en Python       : {tiempo_total_ms:.4f} ms")
    print(f"Tiempo promedio por fila     : {tiempo_promedio_ms:.6f} ms\n")

    # 5. Anexar reporte al archivo tiempos_n100.txt
    with open("tiempos_n100.txt", "a", encoding="utf-8") as f:
        f.write(f"[Python] Repeticiones: {repeticiones}\n")
        f.write(f"[Python] Tiempo total: {tiempo_total_ms:.4f} ms\n")
        f.write(f"[Python] Tiempo promedio generacion n=100: {tiempo_promedio_ms:.6f} ms\n")
        f.write("===================================================\n")

    print("-> Resultados de Python anexados a 'tiempos_n100.txt'")

if __name__ == "__main__":
    main()
