#pragma once
#include "Math.h"
#include <vector>

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);

	void UpdateComponents(float deltaTime);

	virtual void UpdateActor(float deltaTime);

	void AddComponent(class Component* components);
	void RemoveComponent(class Component* component);
// private: I have getter and setters they are dumb
	State mState;
	Vector2 mPosition;
	float mScale;
	float mRotation;
	std::vector<class Component*> mComponents;
	class Game* mGame;
};

