#pragma once
#include "Math.h"


struct Camera {
	float fov;
	float aspect;
	float nearPlane;
	float farPlane;
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
};

struct Texture {
	unsigned int textureID;
	int texWidth;
	int texHeight;
	float scale = 1;
};


struct Sprite {
	unsigned int textureID;
	int texWidth;
	int texHeight;
	float scale = 1;
};

struct Mesh {
	unsigned int arrayID;
	size_t numVerts;
};

struct VertexArray {
	unsigned int arrayID;
	size_t numVerts;
};

struct Shader {
	unsigned int programID;
};

struct Translation {
	Vector3 position;
};

struct Rotation {
	Quaternion rotation;
};

struct Scale {
	float scale;
};

struct Shoot {
  int shootKey;
  float shootCooldown;
};

struct DestroyTag {};

struct Laser {
  float life;
  float radius;
};

struct Move {
  float angularSpeed;
  float forwardSpeed;
};

struct Circle {
  float radius;
};

struct Input {
  float maxFwdSpeed;
  float maxAngSpeed;
  int forwardKey;
  int backKey;
  int clockwiseKey;
  int counterClockwiseKey;
};
