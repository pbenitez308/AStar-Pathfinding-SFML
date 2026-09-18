@echo off
title A* PathFinding - Build and Run

echo.
echo ================================
echo   A* PathFinding - Autorun
echo ================================
echo.

REM Configurar CMake solo si no existe el build
if not exist build\build.ninja (
    echo [1/3] Configurando proyecto...
    cmake -S . -B build -G Ninja

    if errorlevel 1 (
        echo.
        echo ERROR: CMake no pudo configurar el proyecto.
        pause
        exit /b 1
    )
)

echo [2/3] Compilando...
cmake --build build

if errorlevel 1 (
    echo.
    echo ERROR: La compilacion fallo.
    pause
    exit /b 1
)

echo.
echo [3/3] Ejecutando...
echo.

build\AStarPathfinding.exe

if errorlevel 1 (
    echo.
    echo ERROR: El programa termino con un error.
    pause
)

exit /b 0