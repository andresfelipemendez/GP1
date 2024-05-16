#include "Systems.h"
#include "Components.h"
#include "Math.h"
#include "Game.h"

void ProcessInput(GameData& gd, const Uint8* state, MoveData& moveData, InputData& inputData)
{
    
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