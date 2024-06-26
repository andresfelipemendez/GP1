#include "Systems.h"
#include "Components.h"
#include "Math.h"
#include "Game.h"

void InputSystem(entt::registry* registry, const Uint8* keyState)
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

void ShootingSystem(entt::registry* registry, SDL_Renderer* renderer,const Uint8* keyState)
{
    auto shooter = registry->view<Shoot, const Transform>();
    shooter.each([registry, renderer, keyState](Shoot& shoot, const Transform& pos) {
        if (keyState[shoot.shootKey] && shoot.shootCooldown < 0.0f)
        {
            auto laser = registry->create();
            registry->emplace<Laser>(laser, 1.0f, 11.0f);
            registry->emplace<Transform>(laser, pos.x, pos.y, pos.rot);
            registry->emplace<Move>(laser, 0.0f, 800.0f);
            SDL_Texture* tex = GetTexture("Assets/Laser.png", renderer);
            int width, height;
            SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
            registry->emplace<Sprite>(laser, tex, 100, width, height, 1.0f);
            shoot.shootCooldown = 0.5f;
        }
    });
}

void CollisionSystem(entt::registry* registry, float deltaTime)
{
    registry->view <Shoot>().each([deltaTime](Shoot& shoot) {
        shoot.shootCooldown -= deltaTime;
    });

    auto laserBeams = registry->view<Laser>();
    laserBeams.each([registry, deltaTime](const auto entity, Laser& laser) {
        laser.life -= deltaTime;
        if (laser.life < 0.0f)
        {
            registry->emplace<DestroyTag>(entity);
        }
    });

    auto laserBeamsCollision = registry->view<const Laser, const Transform>();
    auto asteroidCollision = registry->view<const Circle, const Transform>();

    for (auto [laserEntity, laser, laserPos] : laserBeamsCollision.each()) {
        for (auto [astEntity, circle, astPos] : asteroidCollision.each()) {
            Vector2 diff = Vector2(laserPos.x, laserPos.y) - Vector2(astPos.x, astPos.y);
            float distSq = diff.LengthSq();

            float radiiSq = laser.radius + circle.radius;
            radiiSq *= radiiSq;
            if (distSq <= radiiSq)
            {
                registry->emplace_or_replace<DestroyTag>(laserEntity);
                registry->emplace_or_replace<DestroyTag>(astEntity);
            }
        }
    }

    auto destroyEntities = registry->view<const DestroyTag>();
    registry->destroy(destroyEntities.begin(), destroyEntities.end());
}



void MovementSystem(entt::registry* registry, float deltaTime) {
    registry->view<const Move, Transform>().each([deltaTime](const Move& move, Transform& _pos) {
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
    auto view = registry->view<const Sprite, const Transform>();
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
