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
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}
 

void SpriteComponent::Draw(Shader* shader)
{
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(mTextWidth),
		static_cast<float>(mTextHeight),
		1.0f
	);

	Matrix4 world = scaleMat * mOwner->GetWorldTransform();

	shader->SetMatrixUniform("uWorldTransform", world);

	glDrawElements(
		GL_TRIANGLES,
		6,
		GL_UNSIGNED_INT,
		nullptr
	);
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	//mTexture = texture;
	//SDL_QueryTexture(texture, nullptr, nullptr, &mTextWidth, &mTextHeight);
}
