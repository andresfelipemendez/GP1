#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawORder): 
	Component(owner, drawORder),
	mTexture(nullptr),
	mDrawOrder(drawORder),
	mTextWidth(64),
	mTextHeight(64)
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

	mTexture->SetActive();
	glDrawElements(
		GL_TRIANGLES,
		6,
		GL_UNSIGNED_INT,
		nullptr
	);
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	mTextWidth = texture->GetWidth();
	mTextHeight = texture->GetHeight();
}
