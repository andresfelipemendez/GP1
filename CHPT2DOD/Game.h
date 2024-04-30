#pragma once
#include <SDL.h>

struct GameData {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Uint32 ticksCount = 0;
	bool isRunning = false;
	bool updatingActors = false;
};

bool Initialize(GameData& gd);
void RunLoop(GameData* gd);
void ShutDown(GameData* gd);

void ProcessInput();
void UpdateGame();
void GenerateOutput();
void LoadData();
void UnloadData();

