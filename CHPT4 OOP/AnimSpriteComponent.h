#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override;
	void SetAnimTexture(const std::vector<SDL_Texture*>& textures);
//private:
	std::vector<SDL_Texture*> mAnimTextures;
	float mCurrentFrame;
	float mAnimFPS;
};

