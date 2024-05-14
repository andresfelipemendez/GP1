#include "Systems.h"
#include "Components.h"

void InputSystem(entt::registry* registry,const Uint8* state)
{
}

void MovementSystem(entt::registry* registry, float deltaTime) {

}

void RenderSystem(SDL_Renderer* renderer, entt::registry* registry)
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
