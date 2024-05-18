#include <iostream>

#include "Game.h"

#include "Components.h"

int main(int argc, char* argv[])
{
    GameData gameData;
    SpriteData spriteData;
    TransformData transformData;
    MoveData moveData;
    InputData inputData;
    ShootData shootData;
    LaserData laserData;
    CircleData circleData;
    EntityIndices entityIndices;

    Registry registry{
        spriteData,
        transformData,
        moveData,
        inputData,
        laserData,
        circleData,
        shootData,
        entityIndices
    };

    bool success = InitializeGame(
        gameData, 
        registry
    );

    if (success) {
        RunLoop(gameData, registry);
    }
   // ShutDown(&gameData);

    return 0;
}