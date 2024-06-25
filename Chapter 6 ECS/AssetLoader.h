#ifndef ASSETLOADER_H
#define ASSETLOADER_H
#include <string>
#include <entt/fwd.hpp>

void LoadScene(entt::registry* registry, const std::string& path);


void LoadMesh();

#endif