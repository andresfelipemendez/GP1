#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawORder): 
	Component(owner, drawORder),
	mTexture(nullptr),
	mDrawOrder(drawORder),
	mTextWidth(0),
	mTextHeight(0)
{
	mOwner->mGame->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->mGame->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{

	if (mTexture)
	{
		SDL_Rect r;
		r.w = static_cast<int>(mTextWidth * mOwner->mScale);
		r.h = static_cast<int>(mTextHeight * mOwner->mScale);
		r.x = static_cast<int>(mOwner->mPosition.x - r.w / 2);
		r.y = static_cast<int>(mOwner->mPosition.y - r.h / 2);

		SDL_RenderCopyEx(
			renderer,
			mTexture,
			nullptr,
			&r,
			-Math::ToDegrees(mOwner->mRotation),
			nullptr,
			SDL_FLIP_NONE
		);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextWidth, &mTextHeight);
}
