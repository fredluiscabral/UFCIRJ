@echo off
setlocal
cd /d "%~dp0"
cl /nologo /W4 /TC 01_lockfileex_threads.c /Fe:01_lockfileex_threads.exe
if errorlevel 1 exit /b 1
echo Compilacao concluida. Execute 01_lockfileex_threads.exe
