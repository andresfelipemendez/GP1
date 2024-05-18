#include "Systems.h"
#include "Components.h"
#include "Math.h"
#include "Game.h"
#include <algorithm>

void ProcessInput(Registry& r, const Uint8* state)
{
    size_t i = 0; // player i'ts input 0
    float fwd = 0.0f;
    if (state[r.inputData.forwardKey]) {
       fwd += r.inputData.maxFwdSpeed;
    }
    if (state[r.inputData.backKey]) {
        fwd -= r.inputData.maxFwdSpeed;
    }
    r.moveData.forwardSpeed[i] = fwd;

    float ang = 0.0f;
    if (state[r.inputData.clockwiseKey]) {
        ang += r.inputData.maxAngSpeed;
    }
    if (state[r.inputData.counterClockwiseKey]) {
        ang -= r.inputData.maxAngSpeed;
    }
    r.moveData.angularSpeed[i] = ang;
}

void UpdateGame(Registry& r, float deltaTime)
{
    for (size_t i = 0; i < moveData.angularSpeed.size(); ++i) {
        if (!Math::NearZero(moveData.angularSpeed[i])) {
            transformData.rot[i] += moveData.angularSpeed[i] * deltaTime;
        }

        if (!Math::NearZero(moveData.forwardSpeed[i])) {
            Vector2 pos(transformData.pos[i].x, transformData.pos[i].y);
            Vector2 forward(Math::Cos(transformData.rot[i]), -Math::Sin(transformData.rot[i]));

            pos += forward * moveData.forwardSpeed[i] * deltaTime;

            if (pos.x < 0.0f) pos.x = 1024.0f;
            if (pos.x > 1024.0f) pos.x = 0.0f;
            if (pos.y < 0.0f) pos.y = 768.0f;
            if (pos.y > 768.0f) pos.y = 0.0f;

            transformData.pos[i] = pos;
        }
    }
}

void RenderGame(SDL_Renderer* renderer, Registry& r) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < r.spriteData.textures.size(); ++i) {
        SDL_Rect rec;
        rec.w = static_cast<int>(r.spriteData.texWidths[i] );
        rec.h = static_cast<int>(r.spriteData.texHeights[i] );
        rec.x = static_cast<int>(r.transformData.pos[i].x - rec.w / 2);
        rec.y = static_cast<int>(r.transformData.pos[i].y - rec.h / 2);

        SDL_RenderCopyEx(renderer, r.spriteData.textures[i], nullptr, &rec, -Math::ToDegrees(r.transformData.rot[i]), nullptr, SDL_FLIP_NONE);
    }

    SDL_RenderPresent(renderer);
}

void CollisionSystem(
    TransformData& transformData,
    LaserData& laserData,
    ShootData& shootData,
    CircleData& circleData,
    EntityIndices& entityIndices, 
    float deltaTime
)
{
    shootData.shootCooldown -= deltaTime;
    

    for (size_t laserIdx = 0; laserIdx < entityIndices.laserIndices.size(); ++laserIdx) {
        int laserEntity = entityIndices.laserIndices[laserIdx];
        float laserX = transformData.pos[laserEntity].x;
        float laserY = transformData.pos[laserEntity].y;
        float laserRadius = laserData.radius[laserEntity];

        for (size_t asteroidIdx = 0; asteroidIdx < entityIndices.asteroidIndices.size(); ++asteroidIdx) {
            int asteroidEntity = entityIndices.asteroidIndices[asteroidIdx];
            float asteroidX = transformData.pos[asteroidEntity].x;
            float asteroidY = transformData.pos[asteroidEntity].y;
            float asteroidRadius = circleData.radius[asteroidEntity];

            float distSq = (laserX - asteroidX) * (laserX - asteroidX) + (laserY - asteroidY) * (laserY - asteroidY);
            float radiiSq = (laserRadius + asteroidRadius) * (laserRadius + asteroidRadius);

            if (distSq <= radiiSq) {
                entityIndices.destroyIndices.push_back(laserEntity);
                entityIndices.destroyIndices.push_back(asteroidEntity);
                break; // Once the laser hits an asteroid, stop checking other asteroids
            }
        }
    }

    
}

void RemoveEntities(
    TransformData& transformData, 
    SpriteData& spriteData, 
    MoveData& moveData, 
    InputData& inputData, 
    ShootData& shootData, 
    CircleData& circleData, 
    LaserData& laserData, 
    EntityIndices& entityIndices
)
{

    std::sort(entityIndices.destroyIndices.rbegin(), entityIndices.destroyIndices.rend());

}
