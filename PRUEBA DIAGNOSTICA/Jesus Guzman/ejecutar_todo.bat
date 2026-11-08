@echo off
echo ===================================================
echo   PRUEBA DIAGNOSTICA - LENGUAJES Y COMPILADORES
echo   Estudiante: Jesus Guzman
echo ===================================================

echo.
echo [1/3] Ejecutando Ejercicio 1 (Ajedrez en Lenguaje C)...
cd "ejercicio 1"
gcc -mconsole ajedrez.c -o ajedrez.exe
.\ajedrez.exe
cd ..

echo.
echo [2/3] Ejecutando Ejercicio 2 (Triangulo de Pascal n=100 en C y Python)...
cd "ejercicio 2"
gcc -mconsole pascal.c -o ejercicio2.exe
.\ejercicio2.exe
python pascal.py
cd ..

echo.
echo [3/3] Ejecutando Ejercicio 3 (Traductor Lexico en Lenguaje C)...
cd "ejercicio 3"
gcc -mconsole traductor.c -o traductor.exe
.\traductor.exe entrada.c salida_espanol.c
cd ..

echo.
echo ===================================================
echo   TODOS LOS EJERCICIOS EJECUTADOS CON EXITO
echo ===================================================
pause
