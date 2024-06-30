#ifndef ASSETLOADER_H
#define ASSETLOADER_H
#include <string>
#include <tuple>
#include <entt/fwd.hpp>
#include "Components.h"

struct ImageFile {
    int width;
    int height;
    int channels;
    unsigned char* data;
    ImageFile(const std::string& fileName);
    ~ImageFile();
};

void LoadScene(entt::registry* registry, const std::string& path);


Mesh LoadOBJMesh(const std::string& path);
Mesh LoadGLTFMesh(const std::string& path);

#endif