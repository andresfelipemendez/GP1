#pragma once
#include "entt/entt.hpp"

#include "Components.h"
#include <vector>

void LoadMesh(entt::registry *registry, std::vector<float> vertices,
              std::vector<unsigned int> indices);
uint32_t UploadMeshToGPU(std::vector<uint32_t> indices, std::vector<float> vertices,
    size_t stride);

Texture LoadTexture(const std::string& fileName);
Shader LoadShader(const std::string& vertexShader, const std::string& fragmentShader);
void DrawOpaque();
void DrawMesh(size_t numVertices);
void SetShaderActive(unsigned int shaderProgram);
void SetVerticesActive(unsigned int vertexID);
void SetTextureActive(unsigned int textureID);
void SetMatrixUniform(Shader shader, const char* uniformName, const class Matrix4* matrix);