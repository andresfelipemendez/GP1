#include "MoveComponent.h"
#include "Math.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder):
	Component(owner, updateOrder),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed)) {
		float rot = mOwner->mRotation;
		rot += mAngularSpeed * deltaTime;
		mOwner->mRotation = rot;
	}

	if (!Math::NearZero(mForwardSpeed))
	{
		Vector2 pos = mOwner->mPosition;
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		if (pos.x < 0.0F) {
			pos.x = 1022.0f;
		}
		else if (pos.x > 1024.0f) {
			pos.x = 2.0f;
		}

		if (pos.y < 0.0F) {
			pos.y = 766.0f;
		}
		else if (pos.y > 768) {
			pos.y = 2.0f;
		}

	}
}
