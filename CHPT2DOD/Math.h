#pragma once


namespace Math
{
	const float Pi = 3.1415926535f;

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}
}
struct Vector2
{
	float x;
	float y;
};