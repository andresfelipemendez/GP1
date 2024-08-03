#pragma once
#include "SDL_Image.h"
#include "Actor.h"

struct Sprite {
  int actor;
  int drawOrder;
  SDL_Texture* mTexture;
};

strict SpriteData {
  Transform t;
  Sprite s;
}

std::vector<SpriteData> sprites;

void DrawSprites(SDL_Renderer* renderer, components...)
{
  SDL_Rect r;
  for(const auto& sprite : sprites) {

    if()
    r.w = static_cast<int>(mTextWidth * mScale);
    r.h = static_cast<int>(mTextHeight * mScale);
    r.x = static_cast<int>(mPosition.x - r.w / 2);
    r.y = static_cast<int>(mPosition.y - r.h / 2);

    SDL_RenderCopyEx(
      renderer,
      mTexture,
      nullptr,
      &r,
      -Math::ToDegrees(mRotation),
      nullptr,
      SDL_FLIP_NONE
    ); 
  }

}
