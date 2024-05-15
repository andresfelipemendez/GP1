#pragma once

struct PlayerInput {
	float mRightSpeed;
	float mDownSpeed;
};

struct BGTexture
{
	SDL_Texture* mTexture;
	Vector2 mOffest;
	float mScrollSpeed;
};

struct AnimSprite {
	SDL_Texture* animFrames;
	size_t numFrames;
	float currentFrame;
	float animFPS;
};