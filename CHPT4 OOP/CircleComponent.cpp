#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner) :
    Component(owner), mRadius(0.0f)
{

}

float CircleComponent::GetRadius() const
{
    return mOwner->mScale * mRadius;
}

const Vector2& CircleComponent::GetCenter() const
{
    return mOwner->mPosition;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
    Vector2 diff = a.GetCenter() - b.GetCenter();
    float distSq = diff.LengthSq();

    float radiiSq = a.GetRadius() + b.GetRadius();
    radiiSq *= radiiSq;

    return distSq <= radiiSq;
}
