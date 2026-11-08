# Script de Automatizacion Windows - Prueba Diagnostica (Jose Miserol)
$ErrorActionPreference = "Stop"
$raiz = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $raiz

Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "  PRUEBA DIAGNOSTICA - LENGUAJES Y COMPILADORES" -ForegroundColor Cyan
Write-Host "  Estudiante: Jose Miserol (Grupo: Binarios en Fuga)" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan

# 1. EJERCICIO 1
Write-Host "`n---------------------------------------------------" -ForegroundColor Yellow
Write-Host "[1/3] Ejecutando Ejercicio 1 (Caballo en Python)..." -ForegroundColor Yellow
Write-Host "---------------------------------------------------" -ForegroundColor Yellow
Set-Location (Join-Path $raiz "Ejercicio 1")
python caballo.py movimientos.txt

# 2. EJERCICIO 2
Write-Host "`n---------------------------------------------------" -ForegroundColor Yellow
Write-Host "[2/3] Ejecutando Ejercicio 2 (Pascal C vs Python)..." -ForegroundColor Yellow
Write-Host "---------------------------------------------------" -ForegroundColor Yellow
Set-Location (Join-Path $raiz "Ejercicio 2")
powershell -ExecutionPolicy Bypass -File .\comparar_python_vs_c.ps1

# 3. EJERCICIO 3
Write-Host "`n---------------------------------------------------" -ForegroundColor Yellow
Write-Host "[3/3] Ejecutando Ejercicio 3 (Traductor Lexico C)..." -ForegroundColor Yellow
Write-Host "---------------------------------------------------" -ForegroundColor Yellow
Set-Location (Join-Path $raiz "Ejercicio 3")
gcc -Wall -o traductor.exe traductor.c
.\traductor.exe ejemplo.c

Set-Location $raiz
Write-Host "`n===================================================" -ForegroundColor Green
Write-Host "  TODOS LOS EJERCICIOS COMPLETADOS CON EXITO" -ForegroundColor Green
Write-Host "===================================================" -ForegroundColor Green