#pragma once

#include <vector>
#include "Math.h"

struct transform {
	Vector2 mPosition;
	float mScale;
	float mRotation;
};

std::vector <int> activeActors;
std::vector <int> pausedActors;
std::vector <int> deadActors;

std::vector<transform> transforms;