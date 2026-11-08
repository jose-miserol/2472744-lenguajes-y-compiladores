# Comparar tiempos de ejecucion entre Python y C para n=100 (Triangulo de Pascal)
$ErrorActionPreference = "Stop"
$directorio = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $directorio

Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host "  EJERCICIO 2: COMPARACION PYTHON VS C (n = 100)" -ForegroundColor Cyan
Write-Host "  Estudiante: Jose Miserol" -ForegroundColor Cyan
Write-Host "==========================================================" -ForegroundColor Cyan

# 1. Compilar version en C
Write-Host "`n[1/3] Compilando pascal.c con optimizacion -O2..." -ForegroundColor Yellow
gcc -O2 -Wall -o pascal.exe pascal.c
if ($LASTEXITCODE -ne 0) {
    Write-Error "Fallo la compilacion de pascal.c"
}
Write-Host "-> Compilacion exitosa: pascal.exe generado." -ForegroundColor Green

# 2. Ejecutar prueba en Python (10,000 repeticiones)
Write-Host "`n[2/3] Ejecutando version en Python (n=100, 10,000 repeticiones)..." -ForegroundColor Yellow
$salidaPython = python pascal.py 100 1 10000
$lineaTiempoPy = ($salidaPython | Select-String "Tiempo promedio")
Write-Host $lineaTiempoPy -ForegroundColor White

# 3. Ejecutar prueba en C (10,000 repeticiones)
Write-Host "`n[3/3] Ejecutando version en C (n=100, 10,000 repeticiones)..." -ForegroundColor Yellow
$salidaC = .\pascal.exe 100 1 10000
$lineaTiempoC = ($salidaC | Select-String "Tiempo promedio")
Write-Host $lineaTiempoC -ForegroundColor White

# 4. Generar reporte tiempos_n100.txt
$archivoReporte = Join-Path $directorio "tiempos_n100.txt"
$reporte = @"
===================================================
COMPARATIVA DE TIEMPOS n=100 (C vs PYTHON)
Estudiante: Jose Miserol
===================================================
Parametros: n = 100, x = 1, Repeticiones = 10000

[Python]
$lineaTiempoPy

[C]
$lineaTiempoC
===================================================
"@

$reporte | Out-File -FilePath $archivoReporte -Encoding utf8
Write-Host "`n==========================================================" -ForegroundColor Green
Write-Host "-> Reporte guardado con exito en 'tiempos_n100.txt'" -ForegroundColor Green
Write-Host "==========================================================" -ForegroundColor Green

