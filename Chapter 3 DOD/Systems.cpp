#include "Systems.h"
#include "Components.h"
#include "Math.h"
void InputSystem(entt::registry* registry,const Uint8* keyState)
{
    auto input = registry->view<const Input, Move>();
    input.each([keyState](const Input& input, Move& move) {
        float forwardSpeed = 0.0f;
        if (keyState[input.forwardKey])
        {
            forwardSpeed += input.maxFwdSpeed;
        }
        if (keyState[input.backKey])
        {
            forwardSpeed -= input.maxFwdSpeed;
        }
        move.forwardSpeed = forwardSpeed;

        float angularSpeed = 0.0f;
        if (keyState[input.clockwiseKey])
        {
            angularSpeed += input.maxAngSpeed;
        }
        if (keyState[input.counterClockwiseKey])
        {
            angularSpeed -= input.maxAngSpeed;
        }
        move.angularSpeed = angularSpeed;
    });
}

void MovementSystem(entt::registry* registry, float deltaTime) {
    registry->view<const Move, Position>().each([deltaTime](const Move& move, Position& _pos) {
        if (!Math::NearZero(move.angularSpeed)) {
            float rot = _pos.rot;
            rot += move.angularSpeed * deltaTime;
            _pos.rot = rot;
        }

        if (!Math::NearZero(move.forwardSpeed))
        {
            Vector2 pos;
            pos.x = _pos.x;
            pos.y = _pos.y;
            Vector2 forward;
            forward.x = Math::Cos(_pos.rot); 
            forward.y = -Math::Sin(_pos.rot);

            pos += forward * move.forwardSpeed * deltaTime;

            if (pos.x < 0.0F) {
                pos.x = 1022.0f;
            }
            else if (pos.x > 1024.0f) {
                pos.x = 2.0f;
            }

            if (pos.y < 0.0F) {
                pos.y = 766.0f;
            }
            else if (pos.y > 768) {
                pos.y = 2.0f;
            }

            _pos.x = pos.x;
            _pos.y = pos.y;
        }
    });
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
            -Math::ToDegrees(pos.rot),
            nullptr,
            SDL_FLIP_NONE
        );
    });
}
