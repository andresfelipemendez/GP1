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

bool InitializeGame(GameData& gd, SpriteData& spriteData, TransformData& transformData,
	MoveData& moveData, InputData& inputData, ShootData& shootData,
	CircleData& circleData, EntityIndices& entityIndices);

void LoadData(GameData& gd, SpriteData& spriteData, TransformData& transformData,
	MoveData& moveData, InputData& inputData, ShootData& shootData,
	CircleData& circleData, EntityIndices& entityIndices);


void RunLoop(GameData& gd, SpriteData& spriteData, TransformData& transformData, MoveData& moveData, InputData& inputData, LaserData& laserData, CircleData& circleData);


void Input(GameData& gd, MoveData& moveData, InputData& inputData);
void Update(GameData& gd, TransformData& transformData, MoveData& moveData,
	LaserData& laserData, CircleData& circleData);

void GenerateOutput(GameData& gd, SpriteData& spriteData, TransformData& trasnformData);
 

SDL_Texture* GetTexture(const std::string& fileName, SDL_Renderer* renderer);