#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteComponent : public SpriteComponent
{
public:
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
//private:
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffest;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};

