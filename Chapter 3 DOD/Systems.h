#pragma once

#include <SDL.h>
#include "Components.h"
#include "Game.h"

void ProcessInput( Registry& r, const Uint8* state);
void ShootingSystem(Registry& r, SDL_Renderer* renderer, const Uint8* state);
void UpdateGame(Registry& r, float deltaTime);
void RenderGame( SDL_Renderer* renderer, Registry& r);
void CollisionSystem(Registry& r, float deltaTime);
void RemoveEntities(Registry& r);
