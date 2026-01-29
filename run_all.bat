@echo off
setlocal enabledelayedexpansion
REM Script automat pentru compilare si rulare teste (Windows)
REM Salveaza rezultatele separat pentru fiecare test

echo ========================================
echo        BUILDING PROJECT...
echo ========================================

REM 1. Compilare
g++ -Wall -Wextra -std=c++17 main.cpp activitate.cpp racheta.cpp asteroid.cpp piata.cpp misiune.cpp -o program.exe

REM Verificam daca compilarea a reusit
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilarea a esuat! Verifica erorile de mai sus.
    pause
    exit /b 1
)

REM 2. Asiguram folderul output principal
if not exist output mkdir output

echo.
echo ========================================
echo       STARTING ALL TESTS (0-6)         
echo ========================================

REM Bucla care trece prin toate testele de la 0 la 6
FOR %%I IN (0 1 2 3 4 5 6) DO (
    echo [RUNNING TEST %%I]...
    
    REM Ruleaza programul pentru testul curent
    program.exe input\test%%I\rachete.csv input\test%%I\asteroizi.csv input\test%%I\piata.csv
    
    REM Creeaza folder specific pentru acest test (ex: output/test0)
    if not exist output\test%%I mkdir output\test%%I
    
    REM Muta rezultatele generate in folderul specific
    if exist output\misiuni.csv move /Y output\misiuni.csv output\test%%I\ >nul
    if exist output\clasament_rachete.csv move /Y output\clasament_rachete.csv output\test%%I\ >nul
    if exist output\profit_total.csv move /Y output\profit_total.csv output\test%%I\ >nul
    if exist output\profit_pe_resursa.csv move /Y output\profit_pe_resursa.csv output\test%%I\ >nul
    
    echo   -> Rezultate salvate in output\test%%I
    echo ----------------------------------------
)

echo.
echo ========================================
echo       ALL TESTS COMPLETED SUCCESSFULY  
echo ========================================
echo Verificati folderele din "output/" pentru rezultate.
pause