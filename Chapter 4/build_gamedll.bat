@echo off
setlocal enabledelayedexpansion

set SOURCE_DIR=game

set OUTPUT_DLL=build/game.dll

set SOURCES=
for %%f in (%SOURCE_DIR%\*.cpp) do (
    set SOURCES=!SOURCES! "%%f"
)

set INCLUDE_DIRS=/I"..\Lib\entt\include" /I"..\Lib\SDL2\include" /I"..\Lib\SDL2_image\include"

set LIB_DIRS=/LIBPATH:"..\Lib\SDL2\lib\x64" /LIBPATH:"..\Lib\SDL2_image\lib\x64"
set LIBS=OpenGl32.lib SDL2.lib SDL2_image.lib SDL2main.lib

cl /std:c++17 /LD /MDd /Zi /EHsc %SOURCES% %INCLUDE_DIRS% /link /MACHINE:X64 %LIB_DIRS% %LIBS% /DLL /OUT:%OUTPUT_DLL% 
