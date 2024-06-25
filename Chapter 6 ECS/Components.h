#pragma once

struct Camera {
	float fov;
	float aspect;
	float near;
	float far;
};

struct Texture {
	unsigned int textureID;
	int texWidth;
	int texHeight;
	float scale = 1;
};

struct VertexArray {
	unsigned int arrayID;
	size_t numVerts;
};

struct Shader {
	unsigned int shaderProgram;
};

struct Transform {
  float x;
  float y;
  float rot;
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
