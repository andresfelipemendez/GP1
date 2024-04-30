#include "Game.h"
#include <SDL_image.h>

bool Initialize(GameData& gd)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	gd.window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 2)",
		100,
		100,
		1024,
		768,
		0
	);
	if (!gd.window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	gd.renderer = SDL_CreateRenderer(
		gd.window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!gd.renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());              
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();
	
	gd.ticksCount = SDL_GetTicks();

	return true;
}

void RunLoop(GameData* gd)
{
}

void ShutDown(GameData* gd)
{
}

void ProcessInput()
{
}

void UpdateGame()
{
}

void GenerateOutput()
{
}

void LoadData()
{
  
}

void UnloadData()
{
}
