@echo off
REM Script automat pentru compilare si rulare teste (Windows)

echo ========================================
echo        BUILDING PROJECT...
echo ========================================

REM 1. Compilare
g++ -Wall -Wextra -std=c++17 main.cpp activitate.cpp racheta.cpp asteroid.cpp piata.cpp misiune.cpp -o program.exe

REM Verificam daca compilarea a reusit
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilarea a esuat! Verifica erorile de mai sus.
    exit /b 1
)

REM 2. Creare folder output
if not exist output mkdir output

echo.
echo ========================================
echo       STARTING ALL TESTS (0-6)         
echo ========================================

echo [RUNNING TEST 0]
program.exe input\test0\rachete.csv input\test0\asteroizi.csv input\test0\piata.csv
echo.

echo [RUNNING TEST 1]
program.exe input\test1\rachete.csv input\test1\asteroizi.csv input\test1\piata.csv
echo.

echo [RUNNING TEST 2]
program.exe input\test2\rachete.csv input\test2\asteroizi.csv input\test2\piata.csv
echo.

echo [RUNNING TEST 3]
program.exe input\test3\rachete.csv input\test3\asteroizi.csv input\test3\piata.csv
echo.

echo [RUNNING TEST 4]
program.exe input\test4\rachete.csv input\test4\asteroizi.csv input\test4\piata.csv
echo.

echo [RUNNING TEST 5]
program.exe input\test5\rachete.csv input\test5\asteroizi.csv input\test5\piata.csv
echo.

echo [RUNNING TEST 6]
program.exe input\test6\rachete.csv input\test6\asteroizi.csv input\test6\piata.csv

echo.
echo ========================================
echo       ALL TESTS COMPLETED SUCCESSFULY  
echo ========================================
pause