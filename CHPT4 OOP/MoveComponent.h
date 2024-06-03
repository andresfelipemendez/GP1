#pragma once
#include "MoveComponent.h"
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

// private:
	//(radians/seconds)
	float mAngularSpeed;
	float mForwardSpeed;
};

