del ..\kbdlk.dll ..\build%BUILD_ALT_DIR%.*
copy ..\..\src\*.c .
copy ..\..\src\*.h .
build -cZ
move obj%BUILD_ALT_DIR%\i386\kbdlk.dll ..
move build%BUILD_ALT_DIR%.* ..
rmdir /s /q obj obj%BUILD_ALT_DIR%
del *.c *.h
if not "%1" == "install" goto noinstall
echo Install ..\kbdlk.inf
%WINDIR%\System32\rundll32.exe setupapi,InstallHinfSection DefaultInstall 132 ..\kbdlk.inf
:noinstall
