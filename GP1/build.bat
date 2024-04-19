@echo off

set SDL2_DIR=C:\User\fmend\lib\SDL2
set INCLUDE=%SDL2_DIR%\include
set LIB=%SDL2_DIR%\lib\x64

cl /EHsc /I%INCLUDE% /Fe:game.exe main.cpp /link /LIBPATH:%LIB% SDL2.lib SDL2main.lib /SUBSYSTEM:CONSOLE

