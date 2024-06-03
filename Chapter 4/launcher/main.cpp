#include <iostream>
#include <entt/entt.hpp>
#include <string>
#include <filesystem>
#include "../game/Game.h"
#include "hot_reload.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::string directory_to_watch_str = getCurrentWorkingDirectory();

    if (directory_to_watch_str.empty()) {
        return 1;
    }

    // Constructing paths for DLLs (or shared libraries on Linux)
    std::string originalDLLpath = directory_to_watch_str + "/game.dll";
    std::string copyDLLpath = directory_to_watch_str + "/game_copy.dll";

    if (!fs::exists(originalDLLpath)) {
        std::cerr << "Original DLL not found: " << originalDLLpath << '\n';
        return -1;
    } else {
        fs::copy(originalDLLpath, copyDLLpath, fs::copy_options::overwrite_existing);
    }

    if (!loadLibrary(copyDLLpath)) {
        return -1;
    }

    InitFunc init = getInitFunction();
    UpdateFunc update = getUpdateFunction();
    ShutdownFunc shutdown = getShutdownFunction();

    if (!init || !update || !shutdown) {
        std::cerr << "Unable to find necessary functions in the library" << std::endl;
        unloadLibrary();
        return -1;
    }

    GameData gameData;
    entt::registry registry;

    if (init(&gameData, &registry)) {
        update(&gameData, &registry);
    }
    shutdown(&gameData);

    unloadLibrary();

    return 0;
}
