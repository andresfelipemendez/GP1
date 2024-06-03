#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(Actor* owner) : 
	MoveComponent(owner),
	mForwardKey(0),
	mBackKey(0),
	mClockwiseKey(0),
	mCounterClockwiseKey(0),
	mMaxForwardSpeed(0),
	mMaxAngularSpeed(0)
{
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	float forwardSpeed = 0.0f;

	if (keyState[mForwardKey])
	{
		forwardSpeed += mMaxForwardSpeed;
	}
	if (keyState[mBackKey])
	{
		forwardSpeed -= mMaxForwardSpeed;
	}
	mForwardSpeed = forwardSpeed;

	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	mAngularSpeed = angularSpeed;
}
