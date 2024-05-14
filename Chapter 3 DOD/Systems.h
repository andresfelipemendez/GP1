#pragma once

#include <SDL.h>
#include <entt/entt.hpp>

void InputSystem(entt::registry* registry,const Uint8* state);
void MovementSystem(entt::registry* registry, float deltaTime);
void RenderSystem(SDL_Renderer* renderer,entt::registry* registry);