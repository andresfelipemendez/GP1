#pragma once
#include <entt/entity/fwd.hpp>
#include <SDL.h>
#include <unordered_map>
#include <string>

struct AnimatedSprite
{
	int entityIndex;
	int numFrames;
	float frameRate;
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

struct Position
{
	float x;
	float y;
};

struct BGSprite
{
	SDL_Texture* texture;
	float offsetX;
	float offsetY;
	float screenSizeX;
	float screenSizeY;
	float mScrollSpeed;
};

struct Input {
	float velX;
	float velY;
};

void UpdateBackGroundSprites(entt::registry* registry, float deltaTime);
void RunAnimationSystem(entt::registry* registry, float deltaTime);
void UpdateInputSystem(entt::registry* registry, const uint8_t* state);
void UpdateInput(entt::registry* registry, float deltaTime);

void Draw(SDL_Renderer* renderer, entt::registry* registry);

SDL_Texture* GetFrame(int entity, int frame);
void SetFrame(int entity, int frame, SDL_Texture* texture);
