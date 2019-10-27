@ECHO OFF
SETLOCAL
IF NOT "%1"=="" GOTO BUILD
echo "Usage: BUILDALL.BAT NAME [debug | keep]"
GOTO END

:BUILD
IF NOT EXIST build MKDIR built
CHDIR built
echo Build i386
IF NOT EXIST i386 MKDIR i386
CHDIR i386
call %~dp0buildarch.bat i386 ..\..\src\%1 %2
echo Build ia64
IF NOT EXIST ..\ia64 MKDIR ..\ia64
CHDIR ..\ia64
call %~dp0buildarch.bat ia64 ..\..\src\%1 %2
echo Build amd64
IF NOT EXIST ..\amd64 MKDIR ..\amd64
CHDIR ..\amd64
call %~dp0buildarch.bat amd64 ..\..\src\%1 %2
echo Build wow64
IF NOT EXIST ..\wow64 MKDIR ..\wow64
CHDIR ..\wow64
call %~dp0buildarch.bat wow64 ..\..\src\%1 %2
CHDIR ..\..\setup
XCOPY * ..\built\ /Y
rem ECHO F | XCOPY "%ProgramFiles(x86)%\Microsoft Keyboard Layout Creator 1.4\bin\i386\kbdsetup.exe" ..\built\setup.exe /Y
:END
ENDLOCAL
