#pragma once

struct Shader {

};

struct Sprite
{
	SDL_Texture* texture;
	int drawOrder;
	int texWidth;
	int texHeight;
	float scale;
};

struct Transform
{
	float x;
	float y;
	float rot;
};

struct Shoot {
	int shootKey;
	float shootCooldown;
};

struct DestroyTag {

};

struct Laser {
	float life;
	float radius;
};

struct Move {
	float angularSpeed;
	float forwardSpeed;
};

struct Circle {
	float radius;
};

struct Input {
	float maxFwdSpeed;
	float maxAngSpeed;
	int forwardKey;
	int backKey;
	int clockwiseKey;
	int counterClockwiseKey;
};