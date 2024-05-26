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
    // laserIndicesStart = player + asteroidCount 
    const auto& shipPos = r.transformData.pos[r.entityIndices.shipIndex];
    const auto& shipRot = r.transformData.rot[r.entityIndices.shipIndex];
    r.entityIndices.laserIndices.emplace_back(21 + static_cast<int>(r.entityIndices.laserIndices.size()));
    r.moveData.angularSpeed.emplace_back(0.0f);
    r.moveData.forwardSpeed.emplace_back(800.0f);
    r.transformData.pos.emplace_back(shipPos.x, shipPos.y);
    r.transformData.rot.emplace_back(shipRot);
    r.laserData.life.emplace_back(11.0f);
    r.laserData.radius.emplace_back(1.0f);
    SDL_Texture* tex = GetTexture("Assets/Laser.png", renderer);
    int width, height;
    SDL_QueryTexture(tex,nullptr,nullptr,&width,&height);
    r.spriteData.textures.emplace_back(tex);
    r.spriteData.texWidths.emplace_back(width);
    r.spriteData.texHeights.emplace_back(height);
    r.shootData.shootCooldown = 0.5f;
  }
}

void UpdateGame(Registry& r, float deltaTime)
{
    r.shootData.shootCooldown -= deltaTime;

    for (size_t i = 0; i < r.moveData.angularSpeed.size(); ++i) {
        if (!Math::NearZero(r.moveData.angularSpeed[i])) {
            r.transformData.rot[i] += r.moveData.angularSpeed[i] * deltaTime;
        }

        if (!Math::NearZero(r.moveData.forwardSpeed[i])) {
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

void RenderSprite(size_t index, Registry& r, SDL_Renderer* renderer) {
    SDL_Rect rec;
    rec.w = static_cast<int>(r.spriteData.texWidths[index]);
    rec.h = static_cast<int>(r.spriteData.texHeights[index]);
    rec.x = static_cast<int>(r.transformData.pos[index].x - rec.w / 2);
    rec.y = static_cast<int>(r.transformData.pos[index].y - rec.h / 2);
    SDL_RenderCopyEx(renderer, r.spriteData.textures[index], nullptr, &rec, -Math::ToDegrees(r.transformData.rot[index]), nullptr, SDL_FLIP_NONE);
}

void RenderGame(SDL_Renderer* renderer, Registry& r) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderSprite(r.entityIndices.shipIndex, r, renderer);

    for (size_t asteroidIndex : r.entityIndices.asteroidIndices)
    {
        RenderSprite(asteroidIndex, r, renderer);
    }
    for (size_t laserIndex : r.entityIndices.laserIndices) {
        RenderSprite(laserIndex, r, renderer);
    }
    SDL_RenderPresent(renderer);
}

void CollisionSystem(Registry& r, float deltaTime)
{
    r.shootData.shootCooldown -= deltaTime;
    for (size_t laserIdx = 0; laserIdx <r.entityIndices.laserIndices.size(); ++laserIdx) {
        size_t laserIndex = r.entityIndices.laserIndices[laserIdx];
        size_t laserDataIndex = laserIndex - r.entityIndices.laserOffset;
        float laserX = r.transformData.pos[laserIndex].x;
        float laserY = r.transformData.pos[laserIndex].y;
        float laserRadius = r.laserData.radius[laserDataIndex];

        for (size_t asteroidIdx = 0; asteroidIdx <r.entityIndices.asteroidIndices.size(); ++asteroidIdx) {
            int asteroidEntity = r.entityIndices.asteroidIndices[asteroidIdx];
            size_t circleDataIndex = asteroidEntity - r.entityIndices.asteroidOffset;
            float asteroidX = r.transformData.pos[asteroidEntity].x;
            float asteroidY = r.transformData.pos[asteroidEntity].y;
            float asteroidRadius = r.circleData.radius[circleDataIndex];

            float distSq = (laserX - asteroidX) * (laserX - asteroidX) + (laserY - asteroidY) * (laserY - asteroidY);
            float radiiSq = (laserRadius + asteroidRadius) * (laserRadius + asteroidRadius);

            if (distSq <= radiiSq) {
               r.entityIndices.destroyAsteroidIndices.push_back(asteroidIdx);
               r.entityIndices.destroyLaserIndices.push_back(laserIdx);
               break;
            }
        }
    }
}

void RemoveEntities(Registry& r)
{
   // std::sort(entityIndices.destroyIndices.rbegin(),r.entityIndices.destroyIndices.rend());
}
