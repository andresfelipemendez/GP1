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


void ShootingSystem(Registry& r, SDL_Renderer* renderer, const Uint8* state) {
  if(state[r.shootData.shootKey] && r.shootData.shootCooldown < 0.0f) {

    const auto& shipPos = r.transformData.pos[r.entityIndices.shipIndex];
    const auto& shipRot = r.transformData.rot[r.entityIndices.shipIndex];

    int index = r.entityIndices.freeLaserIndices.back();
    r.entityIndices.freeLaserIndices.pop_back();
    int transformIndex = index + r.entityIndices.asteroidOffset;
    r.entityIndices.laserIndices.push_back(index);
    r.transformData.pos[transformIndex] = shipPos;
    r.transformData.rot[transformIndex] = shipRot;
    r.laserData.life[index] = 11.0f;
    r.shootData.shootCooldown = 0.5f;
  }
}

void UpdateGame(Registry& r, float deltaTime)
{
    r.shootData.shootCooldown -= deltaTime;

    for (const auto& laserIndex : r.entityIndices.laserIndices) {
        r.laserData.life[laserIndex] -= deltaTime;
        if (r.laserData.life[laserIndex] < 0.0f) {
            r.entityIndices.destroyLaserIndices.emplace(laserIndex);
        }
    }

    for (size_t i = 0; i < r.moveData.angularSpeed.size(); ++i) {
        if (not Math::NearZero(r.moveData.angularSpeed[i])) {
            r.transformData.rot[i] += r.moveData.angularSpeed[i] * deltaTime;
        }

        if (not Math::NearZero(r.moveData.forwardSpeed[i])) {
            Vector2 pos(r.transformData.pos[i].x, r.transformData.pos[i].y);
            Vector2 forward(Math::Cos(r.transformData.rot[i]), -Math::Sin(r.transformData.rot[i]));

            pos += forward * r.moveData.forwardSpeed[i] * deltaTime;

            if (pos.x < 0.0f) pos.x = 1024.0f;
            if (pos.x > 1024.0f) pos.x = 0.0f;
            if (pos.y < 0.0f) pos.y = 768.0f;
            if (pos.y > 768.0f) pos.y = 0.0f;

            r.transformData.pos[i] = pos;
        }
    }
}

void RenderSprite(size_t index, int indexOffset, Registry& r, SDL_Renderer* renderer) {
    SDL_Rect rec;
    index += indexOffset;
    rec.w = static_cast<int>(r.spriteData.texWidths[index]);
    rec.h = static_cast<int>(r.spriteData.texHeights[index]);
    rec.x = static_cast<int>(r.transformData.pos[index].x - rec.w / 2);
    rec.y = static_cast<int>(r.transformData.pos[index].y - rec.h / 2);
    SDL_RenderCopyEx(renderer, r.spriteData.textures[index], nullptr, &rec, -Math::ToDegrees(r.transformData.rot[index]), nullptr, SDL_FLIP_NONE);
}

void RenderGame(SDL_Renderer* renderer, Registry& r) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderSprite(r.entityIndices.shipIndex,0, r, renderer);

    for (size_t asteroidIndex : r.entityIndices.asteroidIndices)
    {
        RenderSprite(asteroidIndex, 1, r, renderer);
    }
    for (size_t laserIndex : r.entityIndices.laserIndices) {
           RenderSprite(laserIndex, r.entityIndices.asteroidOffset, r, renderer);
    }
    SDL_RenderPresent(renderer);
}

void CollisionSystem(Registry& r, float deltaTime)
{
    r.shootData.shootCooldown -= deltaTime;
    for (const auto& laserIndex : r.entityIndices.laserIndices) {
        int transformIndex = laserIndex + r.entityIndices.asteroidOffset;
        float laserX = r.transformData.pos[transformIndex].x;
        float laserY = r.transformData.pos[transformIndex].y;
        float laserRadius = r.laserData.radius[laserIndex];

        for (const auto& asteroidEntityIndex : r.entityIndices.asteroidIndices) {
            size_t asteroidTransformIndex = asteroidEntityIndex + r.entityIndices.shipOffset;
            float asteroidX = r.transformData.pos[asteroidTransformIndex].x;
            float asteroidY = r.transformData.pos[asteroidTransformIndex].y;
            float asteroidRadius = r.circleData.radius[asteroidEntityIndex];

            float distSq = (laserX - asteroidX) * (laserX - asteroidX) + (laserY - asteroidY) * (laserY - asteroidY);
            float radiiSq = (laserRadius + asteroidRadius) * (laserRadius + asteroidRadius);

            if (distSq <= radiiSq) {
                r.entityIndices.destroyAsteroidIndices.emplace(asteroidEntityIndex);
                r.entityIndices.destroyLaserIndices.emplace(laserIndex);
                break;
            }
        }
    }
}

void RemoveEntities(Registry& r) 
{
    for (const auto& find_index : r.entityIndices.destroyLaserIndices) 
    {
        auto it = std::find(r.entityIndices.laserIndices.begin(), r.entityIndices.laserIndices.end(), find_index);
        if (it != r.entityIndices.laserIndices.end()) 
        {
            r.entityIndices.freeLaserIndices.push_back(*it);
            r.entityIndices.laserIndices.erase(it);
        }
    }

    for (const auto& find_index : r.entityIndices.destroyAsteroidIndices) 
    {
        auto it = std::find(r.entityIndices.asteroidIndices.begin(), r.entityIndices.asteroidIndices.end(), find_index);
        if (it != r.entityIndices.asteroidIndices.end())
        {
            r.entityIndices.asteroidIndices.erase(it);
        }
    }

    r.entityIndices.destroyLaserIndices.clear();
    r.entityIndices.destroyAsteroidIndices.clear();
}