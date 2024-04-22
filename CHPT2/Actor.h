#pragma once
#include "Vector2.h"
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

	void AddComponent(class Components* components);
	void RemoveComponent(class Components* component);
// private: I have getter and setters they are dumb
	State mState;
	Vector2 mPosition;
	float mScale;
	float mRotation;
	std::vector<class Component*> mComponents;
	class Game* mGame;
};

