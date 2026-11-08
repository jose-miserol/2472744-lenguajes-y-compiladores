@echo off
setlocal
cd /d "%~dp0"

echo ===================================================
echo   PRUEBA DIAGNOSTICA - LENGUAJES Y COMPILADORES
echo   Estudiante: Jose Miserol (Binarios en Fuga)
echo ===================================================

powershell -ExecutionPolicy Bypass -File "%~dp0ejecutar_todo.ps1"

echo.
pause

