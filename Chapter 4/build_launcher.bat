@echo off
setlocal
set SOURCE=launcher/main.cpp
set TARGET=build/launcher.exe
cl %SOURCE% /Fe%TARGET% /std:c++17
