#pragma once
#include <SDL.h>

struct GameData {
	SDL_Window* window = nullptr;
};

bool Initialize(GameData* gd);
void RunLoop(GameData* gd);
void ShutDown(GameData* gd);

