import time
import subprocess
import os
import sys

def generar_polinomio_python(n):
    if n == 0:
        return [1]
    
    fila_anterior = [1]
    for _ in range(1, n + 1):
        fila_actual = [1]
        for i in range(len(fila_anterior) - 1):
            fila_actual.append(fila_anterior[i] + fila_anterior[i+1])
        fila_actual.append(1)
        fila_anterior = fila_actual
    return fila_anterior

def formatear_polinomio(coefs):
    n = len(coefs) - 1
    terminos = []
    for i, c in enumerate(coefs):
        potencia = n - i
        if potencia == 0:
            terminos.append(f"{c}")
        elif c == 1:
            terminos.append(f"x^{potencia}" if potencia > 1 else "x")
        else:
            terminos.append(f"{c}x^{potencia}" if potencia > 1 else f"{c}x")
    return f"(x+1)^{n} = " + " + ".join(terminos)

def evaluar_polinomio(coefs, x):
    n = len(coefs) - 1
    pasos = []
    resultado = 0
    for i, c in enumerate(coefs):
        potencia = n - i
        termino = c * (x ** potencia)
        resultado += termino
        pasos.append((i + 1, c, x, potencia, termino, resultado))
    return resultado, pasos

def medir_tiempo_python(iteraciones=100, n=100):
    inicio = time.perf_counter()
    for _ in range(iteraciones):
        generar_polinomio_python(n)
    fin = time.perf_counter()
    return fin - inicio

def medir_tiempo_c(directorio_actual, iteraciones=100, n=100):
    exe_c = os.path.join(directorio_actual, "pascal.exe")
    src_c = os.path.join(directorio_actual, "pascal.c")
    
    if not os.path.exists(exe_c) and os.path.exists(src_c):
        subprocess.run(["gcc", "-O2", src_c, "-o", exe_c], check=True)
        
    if os.path.exists(exe_c):
        inicio = time.perf_counter()
        res = subprocess.run([exe_c, str(n), "2", str(iteraciones)], capture_output=True, text=True)
        fin = time.perf_counter()
        
        # Extraer tiempo reportado por el ejecutable C si es posible
        for linea in res.stdout.splitlines():
            if "Tiempo C" in linea and ":" in linea:
                try:
                    tiempo_c_interno = float(linea.split(":")[1].replace("s", "").strip())
                    return tiempo_c_interno
                except ValueError:
                    pass
        return fin - inicio
    return None

def main():
    n = 6
    x = 2
    coefs = generar_polinomio_python(n)
    
    print("--- EJERCICIO 2: POLINOMIO (x+1)^n (PYTHON) ---")
    print(f"Coeficientes para (x+1)^{n}: {coefs}")
    polinomio_str = formatear_polinomio(coefs)
    print(polinomio_str)
    
    res, pasos = evaluar_polinomio(coefs, x)
    print(f"\nEvaluacion paso a paso para x = {x}:")
    for num_paso, c, val_x, pot, termino, acum in pasos:
        print(f"  Paso {num_paso}: {c} * ({val_x}^{pot}) = {termino}  (Suma acumulada = {acum})")
    print(f"Resultado f({x}) = ({x}+1)^{n} = {res}")
    
    # Comparacion de tiempos Python vs C para n=100
    directorio_actual = os.path.dirname(os.path.abspath(__file__))
    iteraciones = 100
    n_medicion = 100
    
    print(f"\n--- COMPARACION DE TIEMPOS PYTHON VS C (n={n_medicion}, {iteraciones} iteraciones) ---")
    tiempo_py = medir_tiempo_python(iteraciones, n_medicion)
    tiempo_c = medir_tiempo_c(directorio_actual, iteraciones, n_medicion)
    
    print(f"Tiempo Python: {tiempo_py:.6f} s ({tiempo_py * 1000:.3f} ms)")
    if tiempo_c is not None:
        print(f"Tiempo C:      {tiempo_c:.6f} s ({tiempo_c * 1000:.3f} ms)")
        if tiempo_c > 0:
            print(f"Factor de aceleracion (C vs Python): C es {tiempo_py / tiempo_c:.2f}x mas rapido")
    
    # Escribir reporte en archivo txt con nombre propio
    ruta_salida = os.path.join(directorio_actual, "salida_tiempos_polinomio.txt")
    with open(ruta_salida, "w", encoding="utf-8") as f:
        f.write("=====================================================\n")
        f.write("REPORTE DE EJECUCION: POLINOMIO Y TIEMPOS (EJ. 2)\n")
        f.write("Estudiante: Franklyn Quintero\n")
        f.write("=====================================================\n")
        f.write(f"Polinomio generado (n={n}): {polinomio_str}\n")
        f.write(f"Evaluacion en x={x}: f({x}) = {res}\n")
        f.write("-----------------------------------------------------\n")
        f.write(f"COMPARACION DE TIEMPOS (n={n_medicion}, {iteraciones} iteraciones):\n")
        f.write(f"  Tiempo Python: {tiempo_py:.6f} s ({tiempo_py * 1000:.3f} ms)\n")
        if tiempo_c is not None:
            f.write(f"  Tiempo C:      {tiempo_c:.6f} s ({tiempo_c * 1000:.3f} ms)\n")
            if tiempo_c > 0:
                f.write(f"  Aceleracion:   C es {tiempo_py / tiempo_c:.2f} veces mas rapido\n")
        f.write("=====================================================\n")
    print(f"\nReporte guardado con exito en: {ruta_salida}")

if __name__ == "__main__":
    main()
