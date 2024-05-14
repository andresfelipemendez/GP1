#pragma once

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

struct Input {
	float velX;
	float velY;

};