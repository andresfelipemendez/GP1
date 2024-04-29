#pragma once
#include "SDL_Image.h"

struct Sprite {
  int actor;
  int drawOrder;
  SDL_Texture* mTexture;
};

std::vector<Sprite> sprites;

void DrawSprites(SDL_Renderer* renderer)
{
  SDL_Rect r;
  for(const auto& sprite : sprites){
    r.w = 
  }

}
