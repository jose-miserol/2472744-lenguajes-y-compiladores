# Script  - Prueba Diagnostica (Yoryelis Ocando)

$ErrorActionPreference = "Stop"

# ---------- Carpetas de cada ejercicio ----------
$raiz       = Split-Path -Parent $PSScriptRoot
$carpeta1   = Join-Path $raiz "Ejercicio 1"
$carpeta2   = Join-Path $raiz "Ejercicio 2"
$carpeta3   = $PSScriptRoot

function Escribir-Titulo($texto) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  $texto" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
}

function Compilar($fuente, $salida, $extra = @()) {
    Write-Host "Compilando $fuente ..."
    & gcc -O2 -Wall -o $salida $fuente @extra
    if ($LASTEXITCODE -ne 0) { throw "Error al compilar $fuente" }
}

# ---------- Verificacion de herramientas ----------
if (-not (Get-Command gcc -ErrorAction SilentlyContinue)) {
    throw "No se encontro gcc. Instale MinGW-w64 y agreguelo al PATH."
}

$python = $null
foreach ($candidato in @("python", "py")) {
    if (Get-Command $candidato -ErrorAction SilentlyContinue) { $python = $candidato; break }
}
if (-not $python) { throw "No se encontro Python en el sistema." }

# ---------- Ejercicio 1: caballo de ajedrez ----------
Escribir-Titulo "Ejercicio 1 - Caballo de ajedrez (C)"
Push-Location $carpeta1
try {
    Compilar "caballo.c" "caballo.exe"
    .\caballo.exe --benchmark | Tee-Object -FilePath "resultados_ejercicio1_c.txt"
}
finally { Pop-Location }

# ---------- Ejercicio 2: triangulo de Pascal ----------
Push-Location $carpeta2
try {
    Escribir-Titulo "Ejercicio 2 - Triangulo de Pascal (C)"
    Compilar "polinomio.c" "polinomio.exe" @("-lm")
    # Opcion 2 del menu (medir tiempo) y luego opcion 0 (salir)
    "2`n0`n" | .\polinomio.exe

    Escribir-Titulo "Ejercicio 2 - Triangulo de Pascal (Python)"
    & $python "polinomio.py" --benchmark
}
finally { Pop-Location }

# ---------- Ejercicio 3: palabras reservadas ----------
Escribir-Titulo "Ejercicio 3 - Palabras reservadas (C)"
Push-Location $carpeta3
try {
    Compilar "memoria.c" "memoria.exe"

    # Programa que se va a analizar: ejemplo.c si existe, de lo contrario memoria.c
    $programaAnalizar = "ejemplo.c"
    if (-not (Test-Path $programaAnalizar)) {
        Write-Host "No existe ejemplo.c, se analizara memoria.c."
        $programaAnalizar = "memoria.c"
    }
    "$programaAnalizar`n" | .\memoria.exe
}
finally { Pop-Location }

# ---------- Resumen ----------
Escribir-Titulo "Archivos generados"
Get-ChildItem -Path $raiz -Recurse -Include "resultados_*.txt", "reporte_palabras_reservadas.txt", "programa_traducido.txt" |
    ForEach-Object { $_.FullName }

Write-Host ""
Write-Host "Proceso terminado." -ForegroundColor Green