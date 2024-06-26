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

      auto texture = _frames.at({ animsprite.entityIndex, frameIndex});
      int width, height;
      SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
      sprite.texture = texture;
      sprite.texWidth = width;
      sprite.texHeight = height;
  });
}

void UpdateBackGroundSprites(entt::registry* registry, float deltaTime)
{
    auto view = registry->view<BGSprite>();
    view.each([deltaTime](BGSprite& bgSprite) {
        bgSprite.offsetX += bgSprite.mScrollSpeed * deltaTime;
        if (bgSprite.offsetX < -bgSprite.screenSizeX) {
            bgSprite.offsetX = bgSprite.screenSizeX - 1;
        }
    });
}

void UpdateInputSystem(entt::registry* registry, const uint8_t* state, float deltaTime) {

}

void UpdateInputSystem(entt::registry* registry, const uint8_t* state) {
    auto bgSprites = registry->view<Input>();
    bgSprites.each([state](Input& input) {
        input.velX = 0.0f;
        input.velY = 0.0f;
        if (state[SDL_SCANCODE_D])
        {
            input.velX += 250.0f;
        }

        if (state[SDL_SCANCODE_A])
        {
            input.velX -= 250.0f;
        }

        if (state[SDL_SCANCODE_S])
        {
            input.velY += 300.0f;
        }

        if (state[SDL_SCANCODE_W])
        {
            input.velY -= 300.0f;
        }
    });
}

void UpdateInput(entt::registry* registry, float deltaTime) {
    auto inputView = registry->view<const Input, Position>();
    inputView.each([deltaTime](const Input& input, Position& pos) {
        Position _pos = pos;
        _pos.x += input.velX * deltaTime;
        _pos.y += input.velY * deltaTime;

        if (_pos.x < 25.0f) {
            _pos.x = 25.0f;
        }
        else if (_pos.x > 500.0f)
        {
            _pos.x = 500.0f;
        }

        if (_pos.y < 25.0f) {
            _pos.y = 25.0f;
        }
        else if (pos.y > 743.0f) {
            _pos.y = 743.0f;
        }
        pos = _pos;
    });
}

void Draw(SDL_Renderer* renderer,entt::registry* registry)
{
    auto bgSprites = registry->view<const BGSprite, const Position>();

    bgSprites.each([renderer](const BGSprite& BGSprite, const Position& pos) {
        SDL_Rect r;
        r.w = static_cast<int>(BGSprite.screenSizeX);
        r.h = static_cast<int>(BGSprite.screenSizeY);

        r.x = static_cast<int>( BGSprite.offsetX);
        r.y = static_cast<int>( 0);

        SDL_RenderCopy(
            renderer,
            BGSprite.texture,
            nullptr,
            &r
        );
    });

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