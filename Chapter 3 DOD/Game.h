#pragma once
#include <SDL.h>
#include <entt/entt.hpp>

struct GameData {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Uint32 ticksCount = 0;
	bool isRunning = false;
	bool updatingActors = false;
};

bool Initialize(GameData* gd, entt::registry* registry);
void RunLoop(GameData* gd, entt::registry* registry);
void ShutDown(GameData* gd);

void ProcessInput(GameData* gd, entt::registry* registry);
void UpdateGame(GameData* gd, entt::registry* registry);
void GenerateOutput(GameData* gd, entt::registry* registry);
void LoadData(GameData* gd, entt::registry* registry);
void UnloadData(GameData* gd);
 

SDL_Texture* GetTexture(const std::string& fileName, SDL_Renderer* renderer);