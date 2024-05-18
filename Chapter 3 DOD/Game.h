#pragma once
#include <SDL.h>
#include "Components.h"
#include <string>

struct GameData {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Uint32 ticksCount = 0;
	bool isRunning = false;
};

bool InitializeGame(GameData& gd, Registry& r);

void LoadData(GameData& gd, Registry& r);


void RunLoop(GameData& gd, Registry& r);


void Input(GameData& gd, Registry& r);

void Update(GameData& gd, Registry& r);

void GenerateOutput(GameData& gd, Registry& r);
 
SDL_Texture* GetTexture(
	const std::string& fileName, 
	SDL_Renderer* renderer
);