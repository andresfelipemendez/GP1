setlocal enabledelayedexpansion

set SOURCE_DIR=engine

set OUTPUT_DLL=engine.dll

set SOURCES=
for %%f in (%SOURCE_DIR%\*.cpp) do (
    set SOURCES=!SOURCES! "%%f"
)

cl /LD %SOURCES% /link /DLL /OUT:%OUTPUT_DLL%
