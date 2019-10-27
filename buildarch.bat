@ECHO OFF
SETLOCAL

set MSKLC=%ProgramFiles(x86)%\Microsoft Keyboard Layout Creator 1.4

IF "%3"=="debug" GOTO DEBUG
SET echo= 
GOTO NODEBUG
:DEBUG
SET echo=echo
:NODEBUG
IF "%2"==""      GOTO BAD
IF "%1"=="i386"  GOTO I386
IF "%1"=="ia64"  GOTO IA64
IF "%1"=="amd64" GOTO AMD64
IF "%1"=="wow64" GOTO WOW64

:BAD
echo "Usage: BUILDARCH.BAT [ i386 | ia64 | amd64 | wow64 ] NAME [debug | keep]"
GOTO END

:I386
set exedir=i386
set libdir=i386
set machine=-MACHINE:IX86
set rdata=.rdata
set defs= 
GOTO BUILD

:IA64
set exedir=i386\IA64
set libdir=ia64
set machine=/MACHINE:IA64
set rdata=.srdata
set defs= 
GOTO BUILD

:AMD64
set exedir=i386\amd64
set libdir=amd64
set machine=-MACHINE:AMD64
set rdata=.rdata
set defs= 
GOTO BUILD

:WOW64
set exedir=i386
set libdir=i386
set machine=-MACHINE:IX86
set rdata=.rdata
set defs=-DBUILD_WOW6432 -D_WOW6432_
GOTO BUILD

:BUILD

rem 27/10/2019 Checked the following is still executed by "KbdUTool -u -m" (v3.40 in MSKLC 1.4)
rem using API Monitor to find _wpopen calls (NB. need to increase maximum length of captured strings).
rem The following commands warn about MSVCRT.lib library format, but that doesn't seem to be a problem.

%echo% "%MSKLC%\bin\%exedir%\cl" -nologo -I"%MSKLC%\inc" -DNOGDICAPMASKS -DNOWINMESSAGES -DNOWINSTYLES -DNOSYSMETRICS -DNOMENUS -DNOICONS -DNOSYSCOMMANDS -DNORASTEROPS -DNOSHOWWINDOW -DOEMRESOURCE -DNOATOM -DNOCLIPBOARD -DNOCOLOR -DNOCTLMGR -DNODRAWTEXT -DNOGDI -DNOKERNEL -DNONLS -DNOMB -DNOMEMMGR -DNOMETAFILE -DNOMINMAX -DNOMSG -DNOOPENFILE -DNOSCROLL -DNOSERVICE -DNOSOUND -DNOTEXTMETRIC -DNOWINOFFSETS -DNOWH -DNOCOMM -DNOKANJI -DNOHELP -DNOPROFILER -DNODEFERWINDOWPOS -DNOMCX -DWIN32_LEAN_AND_MEAN -DRoster -DSTD_CALL -D_WIN32_WINNT=0x0500 /DWINVER=0x0500 -D_WIN32_IE=0x0500 /MD /c /Zp8 /Gy /W3 /WX /Gz /Gm- /EHs-c- /GR- /GF -Z7 /Oxs %defs% "%~dpn2.C"

%echo% "%MSKLC%\bin\i386\rc" -fo "%~n2.res" -r -i"%MSKLC%\inc" -DSTD_CALL -DCONDITION_HANDLING=1 -DNT_UP=1 -DNT_INST=0 -DWIN32=100 -D_NT1X_=100 -DWINNT=1 -D_WIN32_WINNT=0x0500 /DWINVER=0x0400 -D_WIN32_IE=0x0400 -DWIN32_LEAN_AND_MEAN=1 -DDEVL=1 -DFPO=1 -DNDEBUG -l 409 "%~dpn2.RC"

%echo% "%MSKLC%\bin\i386\link" -nologo -merge:.edata=.data -merge:%rdata%=.data -merge:.text=.data -merge:.bss=.data -section:.data,re -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text %machine% -SECTION:INIT,d -OPT:REF -OPT:ICF -IGNORE:4039,4078 -noentry -dll -libpath:"%MSKLC%\lib\%libdir%" -subsystem:native,5.0 -merge:.rdata=.text -PDBPATH:NONE -STACK:0x40000,0x1000 /opt:nowin98 -osversion:4.0 -version:4.0 /release -def:"%~dpn2.DEF" "%~n2.res" "%~n2.obj"

IF "%3"=="keep" GOTO KEEP
DEL %~n2.exp %~n2.lib %~n2.obj %~n2.res
:KEEP

:END
ENDLOCAL
