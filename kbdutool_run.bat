@ECHO OFF
IF "%*"=="" (CALL :HAVEARGS -u -m kbdlk.klc) ELSE (CALL :HAVEARGS %*)
GOTO :EOF

:HAVEARGS
subst Q: "%ProgramFiles(x86)%\Microsoft Keyboard Layout Creator 1.4"
@ECHO ON
Q:\bin\i386\kbdutool %*
@ECHO OFF
subst Q: /d
