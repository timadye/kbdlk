@ECHO OFF
SETLOCAL

SET MSKLC=%ProgramFiles(x86)%\Microsoft Keyboard Layout Creator 1.4

SET ECHO=
SET VERBOSE=
SET KEEP=
SET USECL=
SET QOPT=/Q

:PARMLOOP
IF NOT "%3"=="" (
  IF "%3"=="debug" (
    SET ECHO=ECHO
  ) ELSE IF "%3"=="verbose" (
    SET VERBOSE=1
    SET QOPT=
  ) ELSE IF "%3"=="keep" (
    SET KEEP=1
  ) ELSE IF "%3"=="usecl" (
    SET USECL=1
  ) ELSE (
    GOTO BADPARM
  )
  SHIFT /3
  GOTO :PARMLOOP
)

IF NOT "%USECL%"=="" GOTO :USECL

IF "%1"=="i386" (
  SET KOPT=-x
) ELSE IF "%1"=="ia64"  (
  SET KOPT=-i
) ELSE IF "%1"=="amd64" (
  SET KOPT=-m
) ELSE IF "%1"=="wow64" (
  SET KOPT=-o
) ELSE (
  GOTO BADPARM
)

:: Use kbdutool to compile our own .C file. This requires a .klc file, although the specific content isn't needed for much.
:: Uses a trick from https://github.com/springcomp/optimized-azerty-win/wiki/Build-overview#compiling-the-keyboard-layout-dll
:: where we set the source files read-only, so kbdutool can't overwrite with its own.

IF NOT "%VERBOSE%"=="" @ECHO ON
%ECHO% XCOPY %2.*     /Y /R %QOPT% | FIND /V " File(s) copied"
%ECHO% XCOPY %~dp2*.H /Y    %QOPT% | FIND /V " File(s) copied"
%ECHO% ATTRIB +R %~n2.H
%ECHO% ATTRIB +R %~n2.RC
%ECHO% ATTRIB +R %~n2.C
%ECHO% ATTRIB +R %~n2.DEF
:: kbdutool doesn't work if there are spaces in its path, so mount it as Q:\
%ECHO% subst Q: "%MSKLC%"
%ECHO% Q:\bin\i386\kbdutool -n -v -w -u %KOPT% "%~n2.klc" | FIND /V " : can't open for write."
%ECHO% subst Q: /d
IF "%KEEP%"=="" (
  %ECHO% DEL %~n2.C %~n2.DEF %~n2.RC %~n2.klc *.H /F
)
@ECHO OFF

GOTO END

:: Run the commands that would be done by kbdutool directly...

:USECL
IF "%1"=="i386" (
  SET exedir=i386
  SET libdir=i386
  SET machine=-MACHINE:IX86
  SET rdata=.rdata
  SET defs=
) ELSE IF "%1"=="ia64"  (
  SET exedir=i386\IA64
  SET libdir=ia64
  SET machine=/MACHINE:IA64
  SET rdata=.srdata
  SET defs=
) ELSE IF "%1"=="amd64" (
  SET exedir=i386\amd64
  SET libdir=amd64
  SET machine=-MACHINE:AMD64
  SET rdata=.rdata
  SET defs=
) ELSE IF "%1"=="wow64" (
  SET exedir=i386
  SET libdir=i386
  SET machine=-MACHINE:IX86
  SET rdata=.rdata
  SET defs=-DBUILD_WOW6432 -D_WOW6432_
) ELSE (
  GOTO BADPARM
)

:: 27/10/2019 Checked the following is still executed by "KbdUTool -u -m" (v3.40 in MSKLC 1.4)
:: using API Monitor to find _wpopen calls (NB. need to increase maximum length of captured strings).
:: The following commands warn about MSVCRT.lib library format, but that doesn't seem to be a problem.

IF NOT "%VERBOSE%"=="" @ECHO ON

%ECHO% "%MSKLC%\bin\%exedir%\cl" -nologo -I"%MSKLC%\inc" -DNOGDICAPMASKS -DNOWINMESSAGES -DNOWINSTYLES -DNOSYSMETRICS -DNOMENUS -DNOICONS -DNOSYSCOMMANDS -DNORASTEROPS -DNOSHOWWINDOW -DOEMRESOURCE -DNOATOM -DNOCLIPBOARD -DNOCOLOR -DNOCTLMGR -DNODRAWTEXT -DNOGDI -DNOKERNEL -DNONLS -DNOMB -DNOMEMMGR -DNOMETAFILE -DNOMINMAX -DNOMSG -DNOOPENFILE -DNOSCROLL -DNOSERVICE -DNOSOUND -DNOTEXTMETRIC -DNOWINOFFSETS -DNOWH -DNOCOMM -DNOKANJI -DNOHELP -DNOPROFILER -DNODEFERWINDOWPOS -DNOMCX -DWIN32_LEAN_AND_MEAN -DRoster -DSTD_CALL -D_WIN32_WINNT=0x0500 /DWINVER=0x0500 -D_WIN32_IE=0x0500 /MD /c /Zp8 /Gy /W3 /WX /Gz /Gm- /EHs-c- /GR- /GF -Z7 /Oxs %defs% "%~dpn2.C"

%ECHO% "%MSKLC%\bin\i386\rc" -fo "%~n2.res" -r -i"%MSKLC%\inc" -DSTD_CALL -DCONDITION_HANDLING=1 -DNT_UP=1 -DNT_INST=0 -DWIN32=100 -D_NT1X_=100 -DWINNT=1 -D_WIN32_WINNT=0x0500 /DWINVER=0x0400 -D_WIN32_IE=0x0400 -DWIN32_LEAN_AND_MEAN=1 -DDEVL=1 -DFPO=1 -DNDEBUG -l 409 "%~dpn2.RC"

%ECHO% "%MSKLC%\bin\i386\link" -nologo -merge:.edata=.data -merge:%rdata%=.data -merge:.text=.data -merge:.bss=.data -section:.data,re -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text %machine% -SECTION:INIT,d -OPT:REF -OPT:ICF -IGNORE:4039,4078 -noentry -dll -libpath:"%MSKLC%\lib\%libdir%" -subsystem:native,5.0 -merge:.rdata=.text -PDBPATH:NONE -STACK:0x40000,0x1000 /opt:nowin98 -osversion:4.0 -version:4.0 /release -def:"%~dpn2.DEF" "%~n2.res" "%~n2.obj"

IF "%KEEP%"=="" (
  %ECHO% DEL %~n2.exp %~n2.lib %~n2.obj %~n2.res
)

@ECHO OFF

GOTO END

:BADPARM
ECHO Usage: BUILDARCH.BAT [ i386 ^| ia64 ^| amd64 ^| wow64 ] SRCDIR\NAME [debug] [verbose] [keep] [usecl]
GOTO END

:END
ENDLOCAL
