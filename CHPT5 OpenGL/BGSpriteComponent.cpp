#include "BGSpriteComponent.h"
#include "Actor.h"
#include "Shader.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder):
	SpriteComponent(owner,drawOrder),
	mScrollSpeed(0.0f)
{
}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures)
	{
		bg.mOffest.x += mScrollSpeed * deltaTime;

		if (bg.mOffest.x < -mScreenSize.x)
		{
			bg.mOffest.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(Shader* renderer)
{
	
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mTexture = tex;
		temp.mOffest.x = count * mScreenSize.x;
		temp.mOffest.y = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}
