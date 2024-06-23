#pragma once
#include "Component.h"
#include "SDL_image.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawORder = 100);
	~SpriteComponent();
	virtual void Draw(class Shader* renderer);
	virtual void SetTexture(class Texture* renderer);

//protected:
	int mDrawOrder;
	int mTextWidth;
	int mTextHeight;
protected:

	class Texture* mTexture;
};

