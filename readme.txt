kbdlk 1.0.0.2 - United States-DEC LKxxx-style Keyboard Layout
=============================================================

kbdlk 1.0.0.2 is identical to 1.0.0.1 except that it is now built using the tools that come with the Microsoft Keyboard Layout Creator (MSKLC) 1.4. This comes with compiler and everything needed, so you don't need Visual Studio or the DDK. It also allows the creation of i386, IA64, and AMD64 DLLs.

To build, run

  buildall.bat kbdlk

This runs buildarch.bat for each architecture. buildarch.bat does the same compilation as MSKLC's kbdutool.exe, but uses my old kbdlk.c and kbdlk.h instead of generating one from kbdlk.klc.

This creates a "build" subdirectory. Documentation and installation files are copied from the "setup" subdirectory. setup.exe and the kbdlk_*.msi files were created with MSKLC using kbdlk.klc (just depends on the name - they don't need to be recreated with the DLLs). This is the only use of kbdlk.klc.
