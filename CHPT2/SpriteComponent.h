#pragma once
#include "Component.h"
#include "SDL_image.h"
class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawORder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Renderer* renderer);

	//SDL_Texture* LoadTexture()
//protected:
	SDL_Texture* mTexture;
	int mDrawOrder;
	int mTextWidth;
	int mTextHeight;
};

