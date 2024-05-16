#pragma once

#include <SDL.h>
#include "Components.h"
#include "Game.h"

void ProcessInput(const Uint8* state, MoveData& moveData, InputData& inputData);
void UpdateGame(TransformData& transformData, MoveData& moveData, LaserData& laserData, CircleData& circleData, float deltaTime);
void RenderGame(SDL_Renderer* renderer, const SpriteData& spriteData, const TransformData& transformData);
/*
void ShootingSystem(entt::registry* registry, SDL_Renderer* renderer, const Uint8* keyState);
void InputSystem(entt::registry* registry,const Uint8* state);
void MovementSystem(entt::registry* registry, float deltaTime);
void CollisionSystem(entt::registry* registry, float deltaTime);
void RenderSystem(SDL_Renderer* renderer,entt::registry* registry);
*/