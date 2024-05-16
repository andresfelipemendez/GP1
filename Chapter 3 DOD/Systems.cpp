#include "Systems.h"
#include "Components.h"
#include "Math.h"
#include "Game.h"

void ProcessInput(const Uint8* state, MoveData& moveData, InputData& inputData)
{
    size_t i = 0;
    moveData.forwardSpeed[i] = 0.0f;
    if (state[inputData.forwardKey]) {
        moveData.forwardSpeed[i] = inputData.maxFwdSpeed;
    }
    if (state[inputData.backKey]) {
        moveData.forwardSpeed[i] -= inputData.maxFwdSpeed;
    }
    moveData.angularSpeed[i] = 0.0f;
    if (state[inputData.clockwiseKey]) {
        moveData.angularSpeed[i] = inputData.maxAngSpeed;
    }
    if (state[inputData.counterClockwiseKey]) {
        moveData.angularSpeed[i] -= inputData.maxAngSpeed;
    }
}

void UpdateGame(TransformData& transformData, MoveData& moveData, LaserData& laserData, CircleData& circleData, float deltaTime)
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

void RenderGame(SDL_Renderer* renderer, const SpriteData& spriteData, const TransformData& transformData) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < spriteData.textures.size(); ++i) {
        SDL_Rect r;
        r.w = static_cast<int>(spriteData.texWidths[i] );
        r.h = static_cast<int>(spriteData.texHeights[i] );
        r.x = static_cast<int>(transformData.pos[i].x - r.w / 2);
        r.y = static_cast<int>(transformData.pos[i].y - r.h / 2);

        SDL_RenderCopyEx(renderer, spriteData.textures[i], nullptr, &r, -Math::ToDegrees(transformData.rot[i]), nullptr, SDL_FLIP_NONE);
    }

    SDL_RenderPresent(renderer);
}