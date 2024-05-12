#include "AnimatedSprite.h"
#include "entt/entt.hpp"
#include <iostream>

typedef std::pair<int, int> pair;
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

std::unordered_map<pair, SDL_Texture*, pair_hash> _frames;

SDL_Texture* GetFrame(int entity, int frame) {
    return _frames.at({entity, frame});
}

void SetFrame(int entity, int frame, SDL_Texture* texture) {
    _frames.insert({ {entity, frame}, texture });
}

void RunAnimationSystem(entt::registry* registry, float deltaTime)
{

  auto view = registry->view<AnimatedSprite, Sprite>();
  std::vector<int> listOfIndexesToQuery;
  view.each([&listOfIndexesToQuery, deltaTime](auto& animsprite, auto& sprite){
      animsprite.currentFrame += animsprite.frameRate * deltaTime;
      int frameIndex = (static_cast<int>(animsprite.currentFrame) % animsprite.numFrames) + 1;

      // get texture
      auto texture = _frames.at({ animsprite.entityIndex, frameIndex});
      int width, height;
      SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
      sprite.texture = texture;
      sprite.texWidth = width;
      sprite.texHeight = height;
  });

}

void Draw(SDL_Renderer* renderer,entt::registry* registry)
{
    auto view = registry->view<const Sprite, const Position>();

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