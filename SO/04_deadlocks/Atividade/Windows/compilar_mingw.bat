@echo off
setlocal
cd /d "%~dp0"
gcc -std=c11 -O0 -Wall -Wextra 01_lockfileex_threads.c -o 01_lockfileex_threads.exe
if errorlevel 1 exit /b 1
echo Compilacao concluida. Execute 01_lockfileex_threads.exe
