#ifndef HOT_RELOAD_H
#define HOT_RELOAD_H

#include <string>

typedef bool (*InitFunc)(void*, void*);
typedef void (*UpdateFunc)(void*, void*);
typedef void (*ShutdownFunc)(void*);

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#define getcwd _getcwd
#define PATH_MAX MAX_PATH
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

bool loadLibrary(const std::string& libName);
void unloadLibrary();
InitFunc getInitFunction();
UpdateFunc getUpdateFunction();
ShutdownFunc getShutdownFunction();
std::string getCurrentWorkingDirectory();

#endif // HOT_RELOAD_H
