@ECHO OFF
SETLOCAL

IF "%1"=="-h" (
   ECHO Usage: BUILDALL.BAT NAME [debug] [verbose] [keep] [usecl]
   GOTO END
)
IF "%1"=="" (
   SET NAME=kbdlk
) ELSE (
   SET NAME=%1
)

IF NOT EXIST built MKDIR built
CHDIR built

ECHO Build i386
IF NOT EXIST i386 MKDIR i386
CHDIR i386
CALL %~dp0buildarch.bat i386 ..\..\src\%NAME% %2 %3 %4 %5 %6 %7 %8 %9

ECHO Build ia64
IF NOT EXIST ..\ia64 MKDIR ..\ia64
CHDIR ..\ia64
CALL %~dp0buildarch.bat ia64 ..\..\src\%NAME% %2 %3 %4 %5 %6 %7 %8 %9

ECHO Build amd64
IF NOT EXIST ..\amd64 MKDIR ..\amd64
CHDIR ..\amd64
CALL %~dp0buildarch.bat amd64 ..\..\src\%NAME% %2 %3 %4 %5 %6 %7 %8 %9

ECHO Build wow64
IF NOT EXIST ..\wow64 MKDIR ..\wow64
CHDIR ..\wow64
CALL %~dp0buildarch.bat wow64 ..\..\src\%NAME% %2 %3 %4 %5 %6 %7 %8 %9

CHDIR ..\..\setup
XCOPY * ..\built\ /Y /Q | FIND /V " File(s) copied"

:END
ENDLOCAL
