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

    bool success = InitializeGame(
        gameData, 
        spriteData, 
        transformData,
        moveData,
        inputData,
        shootData,
        circleData,
        entityIndices
    );

    if (success) {
        RunLoop(gameData, spriteData, transformData, moveData, inputData, laserData, circleData);
    }
   // ShutDown(&gameData);

    return 0;
}