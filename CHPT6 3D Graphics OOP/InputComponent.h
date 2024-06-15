#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent :
    public MoveComponent
{
public:
    InputComponent(class Actor* owner);

    void ProcessInput(const uint8_t* keystate) override;

    float mMaxForwardSpeed;
    float mMaxAngularSpeed;

    int mForwardKey;
    int mBackKey;

    int mClockwiseKey;
    int mCounterClockwiseKey;
};

