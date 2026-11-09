import time
import os

def validar_movimientos_caballo(pos_inicial, n, cadena_movimientos):
    def parse_pos(pos_str):
        if len(pos_str) != 2:
            return None
        col_char, fila_char = pos_str[0], pos_str[1]
        if col_char < 'a' or col_char > 'h' or fila_char < '1' or fila_char > '8':
            return None
        return ord(col_char) - ord('a') + 1, int(fila_char)

    pos_actual = parse_pos(pos_inicial)
    if not pos_actual:
        return f"Posicion inicial {pos_inicial} invalida."

    movimientos = cadena_movimientos.strip().split()

    for i, mov in enumerate(movimientos, start=1):
        if not mov.startswith('K') or len(mov) != 3:
            return f"movimiento INCORRECTO falla letra {mov[0] if len(mov)>0 else ''} no pertenece a un caballo." if len(mov) > 0 and not mov.startswith('K') else f"movimiento INCORRECTO falla en el movimiento {i}"
        
        destino_str = mov[1:]
        destino = parse_pos(destino_str)
        
        if not destino:
            return f"movimiento INCORRECTO falla en el movimiento {i}"
        
        dif_col = abs(pos_actual[0] - destino[0])
        dif_fila = abs(pos_actual[1] - destino[1])
        
        es_salto_valido = (dif_col == 2 and dif_fila == 1) or (dif_col == 1 and dif_fila == 2)
        
        if not es_salto_valido:
            return f"movimiento INCORRECTO falla en el movimiento {i}"
        
        pos_actual = destino

    return "movimiento CORRECTO."

def ejecutar_prueba_500():
    pos_inicial = "a2"
    cadena_500 = (
        "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
        "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
        "Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
        "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
        "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
        "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 "
        "Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 "
        "Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 "
        "Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 "
        "Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
        "Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 "
        "Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
        "Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
        "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
        "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
        "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 "
        "Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
        "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 "
        "Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 "
        "Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 "
        "Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 "
        "Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4"
    )
    
    movs = cadena_500.strip().split()
    n = len(movs)
    
    tiempos = []
    for _ in range(10):
        t0 = time.perf_counter()
        res = validar_movimientos_caballo(pos_inicial, n, cadena_500)
        t1 = time.perf_counter()
        tiempos.append(t1 - t0)
    
    tiempo_total = sum(tiempos)
    tiempo_promedio = tiempo_total / 10
    
    print(f"Resultado: {res}")
    print(f"Movimientos evaluados: {n}")
    print(f"Tiempo 10 ejecuciones: {tiempo_total:.6f} s")
    print(f"Tiempo promedio por ejecucion: {tiempo_promedio:.6f} s")
    
    directorio_actual = os.path.dirname(os.path.abspath(__file__))
    ruta_salida = os.path.join(directorio_actual, "salida_tiempos_caballo.txt")
    with open(ruta_salida, "w", encoding="utf-8") as f:
        f.write("=====================================================\n")
        f.write("REPORTE DE EJECUCION: MOVIMIENTOS DE CABALLO (EJ. 1)\n")
        f.write("Estudiante: Franklyn Quintero\n")
        f.write("=====================================================\n")
        f.write(f"Posicion inicial (Ak): {pos_inicial}\n")
        f.write(f"Cantidad de movimientos evaluados: {n}\n")
        f.write(f"Resultado de la validacion: {res}\n")
        f.write("-----------------------------------------------------\n")
        for i, t in enumerate(tiempos, 1):
            f.write(f"  Corrida {i:2d}: {t:.6f} s ({t * 1000:.4f} ms)\n")
        f.write("-----------------------------------------------------\n")
        f.write(f"Tiempo total (10 ejecuciones): {tiempo_total:.6f} s ({tiempo_total * 1000:.4f} ms)\n")
        f.write(f"Tiempo promedio por ejecucion: {tiempo_promedio:.6f} s ({tiempo_promedio * 1000:.4f} ms)\n")
    print(f"Reporte guardado en: {ruta_salida}")

if __name__ == "__main__":
    ejecutar_prueba_500()
