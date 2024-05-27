@echo off
setlocal
set SOURCE=watcher.c
set TARGET=watcher.exe
cl %SOURCE% /Fe%TARGET% 
