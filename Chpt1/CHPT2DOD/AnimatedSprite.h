#pragma once
#include <entt/entity/fwd.hpp>
#include <SDL.h>
#include <unordered_map>
#include <string>

struct AnimatedSprite
{
  std::string name;
	float frameRate;
	int numFrames;
	float currentFrame;
};

struct Sprite
{
	SDL_Texture* texture;
	int drawOrder;
	int texWidth;
	int texHeight;
	float scale;
};

struct position
{
	float x;
	float y;
};



void RunAnimationSystem(entt::registry* registry, float deltaTime);

void Draw(SDL_Renderer* renderer, entt::registry* registry);

SDL_Texture* GetFrame(const std::string& name);
void SetFrame(const std::string& name, SDL_Texture* texture);
