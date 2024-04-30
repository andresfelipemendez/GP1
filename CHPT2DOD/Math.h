#pragma once


namespace Math
{
	const float Pi = 3.1415926535f;

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}
}
class Vector2
{
public:
	float x;
	float y;

	Vector2() :
		x(0.0f),
		y(0.0f)
	{}

	Vector2(float inX, float inY) :
		x(inX),
		y(inY)
	{}

	void Set(float inX, float inY)
	{
		x = inX;
		y = inY;
	}

	static const Vector2 Zero;
};