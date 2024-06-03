@echo off
setlocal
set SOURCE=watcher/watcher.c
set TARGET=build/watcher.exe
set OBJ_DIR=build/
cl %SOURCE% /Fo%OBJ_DIR% /Fe%TARGET% 
