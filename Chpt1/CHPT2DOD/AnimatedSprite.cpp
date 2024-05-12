#include "AnimatedSprite.h"
#include "entt/entt.hpp"
#include <iostream>

std::unordered_map<std::string, SDL_Texture*> _frames;

SDL_Texture* GetFrame(const std::string& name) {
    return _frames.at(name);
}

void SetFrame(const std::string& name, SDL_Texture* texture) {
    _frames.emplace(name, texture);
}

void RunAnimationSystem(entt::registry* registry, float deltaTime)
{

  auto view = registry->view<AnimatedSprite, Sprite>();
  std::vector<int> listOfIndexesToQuery;
  view.each([&listOfIndexesToQuery, deltaTime](auto& animsprite, auto& sprite){
      animsprite.currentFrame += animsprite.frameRate * deltaTime;
      std::string name = animsprite.name;
      name += std::to_string((static_cast<int>(animsprite.currentFrame) % animsprite.numFrames) + 1);

      // get texture
      auto texture = _frames.at(name);
      int width, height;
      SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
      sprite.texture = texture;
      sprite.texWidth = width;
      sprite.texHeight = height;
  });

}

void Draw(SDL_Renderer* renderer,entt::registry* registry)
{
    auto view = registry->view<const Sprite, const position>();

    view.each([renderer](const auto& sprite, const auto& pos) {
        SDL_Rect r;
        r.w = static_cast<int>(sprite.texWidth * sprite.scale);
        r.h = static_cast<int>(sprite.texHeight * sprite.scale);
        r.x = static_cast<int>(pos.x - r.w / 2);
        r.y = static_cast<int>(pos.y - r.h / 2);

        SDL_RenderCopyEx(
            renderer,
            sprite.texture,
            nullptr,
            &r,
            0,
            nullptr,
            SDL_FLIP_NONE
        );
    });
}