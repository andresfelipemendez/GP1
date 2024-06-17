#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"

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

void SpriteComponent::Draw(Shader* renderer)
{

	glDrawElements(
		GL_TRIANGLES,
		6,
		GL_UNSIGNED_INT,
		nullptr
	);
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextWidth, &mTextHeight);
}
