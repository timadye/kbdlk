del ..\kbdlk.dll ..\build.*
copy ..\..\src\*.c .
copy ..\..\src\*.h .
md obj\i386\%DDKBUILDENV%
build -cZ
move obj\i386\%DDKBUILDENV%\kbdlk.dll ..
move build.* ..
rmdir /s /q obj
del *.c *.h
if not "%1" == "install" goto noinstall
echo Install ..\kbdlk.inf
%WINDIR%\System32\rundll32.exe setupapi,InstallHinfSection DefaultInstall 132 ..\kbdlk.inf
:noinstall
