@echo off
rem MREGA launcher for Windows
rem (c) 2020 Franticware
dir /B /O:-N "C:\Program Files (x86)\DOSBox*" > run_win.tmp
dir /B /O:-N "C:\Program Files\DOSBox*" >> run_win.tmp
set /p DBXDIR=<run_win.tmp
del run_win.tmp
"C:\Program Files (x86)\%DBXDIR%\DOSBox.exe" mrega.exe
if %ERRORLEVEL% equ 0 exit 0
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo !! DOSBox has to be installed first.               !!
echo !! Press any key to go to the DOSBox Download page !!
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
pause
start "" https://www.dosbox.com/download.php?main=1
