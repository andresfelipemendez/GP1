#pragma once
#include <entt/entt.hpp>

struct GameData {
	class SDL_Window* window = nullptr;
	class SDL_Renderer* renderer = nullptr;
	uint32_t ticksCount = 0;
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
 

class SDL_Texture* GetTexture(const std::string& fileName, class SDL_Renderer* renderer);