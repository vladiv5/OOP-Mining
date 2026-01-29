@echo off
setlocal enabledelayedexpansion
REM I configured the script to disable command echoing for a cleaner console view.
REM I also enabled delayed expansion to handle variables correctly inside loops.

echo ========================================
echo        BUILDING PROJECT...
echo ========================================

REM 1. Compilation
REM I compile the project using g++ with C++17 standard and enable all warnings to ensure code quality and standard compliance.
g++ -Wall -Wextra -std=c++17 main.cpp activitate.cpp racheta.cpp asteroid.cpp piata.cpp misiune.cpp -o program.exe

REM I check the compiler's exit code. If it's not 0 (success), I pause the script to allow the user to see the error message.
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilarea a esuat! Verifica erorile de mai sus.
    pause
    exit /b 1
)

REM 2. Setup Output Directory
REM I ensure the main output directory exists before running tests to prevent file write errors.
if not exist output mkdir output

echo.
echo ========================================
echo       STARTING ALL TESTS (0-6)         
echo ========================================

REM I loop through all defined test cases (0 to 6) to automate the testing process.
FOR %%I IN (0 1 2 3 4 5 6) DO (
    echo [RUNNING TEST %%I]...
    
    REM I execute the program using the input files specific to the current test index (%%I).
    program.exe input\test%%I\rachete.csv input\test%%I\asteroizi.csv input\test%%I\piata.csv
    
    REM I create a dedicated subfolder for this specific test to keep results organized.
    if not exist output\test%%I mkdir output\test%%I
    
    REM I move the generated CSV reports from the main output folder to the specific test folder.
    REM I use /Y to overwrite existing files without prompting and >nul to suppress move confirmation messages.
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