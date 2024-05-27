#pragma once
#include <vector>
#include <set>
#include "Math.h"

struct SpriteData {
    std::vector<SDL_Texture*> textures;
    std::vector<int> texWidths;
    std::vector<int> texHeights;
};

struct TransformData {
    std::vector<Vector2> pos;
    std::vector<float> rot;
};

struct MoveData {
    std::vector<float> angularSpeed;
    std::vector<float> forwardSpeed;
};

struct InputData {
    float maxFwdSpeed;
    float maxAngSpeed;
    int forwardKey;
    int backKey;
    int clockwiseKey;
    int counterClockwiseKey;
};

struct LaserData {
    std::vector<float> life;
    std::vector<float> radius;
};

struct CircleData {
    std::vector<float> radius;
};

struct ShootData {
    int shootKey;
    float shootCooldown = 0.0f;
};

struct EntityIndices {
    int shipIndex = 0;
    int shipOffset = 1;
    size_t asteroidOffset = 1;
    std::vector<int> asteroidIndices;
    std::vector<int> laserIndices;
    std::vector<int> freeLaserIndices;
    std::set<int> destroyAsteroidIndices;
    std::set<int> destroyLaserIndices;
};

struct Registry {
    SpriteData& spriteData;
    TransformData& transformData;
    MoveData& moveData;
    InputData& inputData;
    LaserData& laserData;
    CircleData& circleData;
    ShootData& shootData;
    EntityIndices& entityIndices;
};
