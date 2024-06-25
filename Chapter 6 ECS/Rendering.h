#pragma once
#include "entt/entt.hpp"

#include "Components.h"
#include <vector>

void LoadMesh(entt::registry *registry, std::vector<float> vertices,
              std::vector<unsigned int> indices);

Texture LoadTexture(const std::string& fileName);
Shader LoadShader(const std::string& vertexShader, const std::string& fragmentShader);

void SetShaderActive(unsigned int shaderProgram);
void SetVerticesActive(unsigned int vertexID);
void SetTextureActive(unsigned int textureID);
void SetMatrixUniform(Shader shader, const char* uniformName, const class Matrix4* matrix);