#include "AssetLoader.h"
#include "Components.h"
#include <entt/entt.hpp>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <toml.h>
#include <SDL.h>
#include <vector>
#include <fstream>
#include <fstream>
#include <iostream>
#include <string>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Renderer.h"
#include <GL/glew.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

ImageFile::ImageFile(const std::string& fileName)
{
    stbi_set_flip_vertically_on_load(true);
    int format;
    data = stbi_load(fileName.c_str(), &width, &height, &format, 0);
    if (!data) {
        SDL_Log("STB failed to load image %s: %s", fileName.c_str(),
            stbi_failure_reason());
    }
    channels = format == 4 ? GL_RGBA : GL_RGB;
}
ImageFile::~ImageFile() {
    stbi_image_free(data);
}


template <typename T>
std::optional<T> get_json_value(const nlohmann::json& j, const std::string& key) {
    if (j.contains(key) && !j[key].is_null()) {
        return j[key].get<T>();
    }
    else {
        std::cerr << "Missing or null field: " << key << std::endl;
        return std::nullopt;
    }
}

void LoadScene(entt::registry* registry, const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    nlohmann::json data = nlohmann::json::parse(f, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Failed to parse JSON from file: " << path << std::endl;
        return;
    }

    f.close();

    if (!data.contains("entities") || !data["entities"].is_array()) {
        std::cerr << "Invalid or missing 'entities' field in JSON." << std::endl;
        return;
    }

    for (auto& entity : data["entities"]) {
        auto e = registry->create();

        for (auto& component : entity["components"]) {
            std::string type = component.value("type", "");
            if (type.empty()) {
                std::cerr << "Component type is missing or empty." << std::endl;
                continue;
            }

            if (type == "camera") {
                auto fov = get_json_value<float>(component, "fov");
                auto _near = get_json_value<float>(component, "near");
                auto _far = get_json_value<float>(component, "far");

                // I should bring these from the scene
                Matrix4 view = Matrix4::CreateLookAt(
                    Vector3::Zero, // camera position
                    Vector3::UnitX, // targe position
                    Vector3::UnitZ // up
                );

                Matrix4 projection = Matrix4::CreatePerspectiveFOV(
                    Math::ToRadians(*fov),
                    1024, // width hardcoded for now
                    768, // height 
                    *_near,
                    *_far
                );

                if (fov && _near && _far) {
                    auto& cam = registry->emplace<Camera>(e);
                    cam.fov = *fov;
                    cam.nearPlane = *_near;
                    cam.farPlane = *_far;
                    cam.viewMatrix = view;
                    cam.projectionMatrix = projection;
                }
                else {
                    std::cerr << "Camera component skipped due to missing data." << std::endl;
                }
            }

            if (type == "translation") {
                if (component["position"].is_array() && component["position"].size() == 3) {
                    auto& pos = registry->emplace<Translation>(e);
					pos.position.x = component["position"][0];
					pos.position.y = component["position"][1];
					pos.position.z = component["position"][2];

                }
                else {
                    std::cerr << "Translation component skipped due to missing data." << std::endl;
                }
            }

            if (type == "rotation") {
                if (component["euler"].is_array() && component["euler"].size() == 3) {
					Vector3 rotEuler;
                    rotEuler.x = component["euler"][0];
                    rotEuler.y = component["euler"][1];
                    rotEuler.z = component["euler"][2];
                    auto& rot = registry->emplace<Rotation>(e);
					rot.rotation = Quaternion::EulerToQuaternion(rotEuler);
				}
				else {
					std::cerr << "Rotation component skipped due to missing data." << std::endl;
                }
            }

            if (type == "movement") {
                registry->emplace<Move>(e);
            }

            if (type == "input") {
                auto& input = registry->emplace<Input>(e);
                // I might need a better way to configure input
                input.forwardKey = SDL_SCANCODE_W;
                input.backKey = SDL_SCANCODE_S;
                input.clockwiseKey = SDL_SCANCODE_D;
                input.counterClockwiseKey = SDL_SCANCODE_A;
                input.maxFwdSpeed = component["forwardSpeed"]; 
                input.maxAngSpeed = component["angularSpeed"];
            }

			if (type == "mesh") {
                if (component["filePath"].is_string()) {
                    std::string path = component["filePath"];
                    path = "Assets/" + path;
                    Mesh vertexArray{};                    
                    if (path.ends_with(".obj") && !LoadOBJMesh(path, vertexArray)) {
                        auto& mesh = registry->emplace<Mesh>(e);
                        mesh.arrayID = vertexArray.arrayID;
                        mesh.numVerts = vertexArray.numVerts;
                    }
                    
				}
				else {
					std::cerr << "Mesh component skipped due to missing data." << std::endl;
				}
			}

            if (type == "model") {
                if (component["filePath"].is_string()) {
                    std::string path = component["filePath"];
                    path = "Assets/" + path;
                    Mesh vertexArray{};
                    if (path.ends_with(".gltf") || path.ends_with(".glb")) {
                        auto& newMesh = registry->emplace<MultiMesh>(e);
                        newMesh.meshes = LoadGLTFMeshes(path);
                    }
                }
                else {
                    std::cerr << "Mesh component skipped due to missing data." << std::endl;
                }
            }

            if (type == "material") {
                if (component["shader"].is_string())
                {
                    std::string path = component["shader"];
                    std::string fragmentPath = "Assets/" + path + ".frag";
                    std::string vertexPath = "Assets/" + path + ".vert";
                    Shader shaderProgramID = LoadShader(vertexPath, fragmentPath);
                    auto& shader = registry->emplace<Shader>(e);
                    shader.programID = shaderProgramID.programID;
                }
            }

            if (type == "texture") {
                if (component["filePath"].is_string()) {
                    std::string path = component["filePath"];
                    path = "Assets/" + path; 
                    Texture t;
                    t = LoadTexture(path);
                    auto& tex = registry->emplace<Texture>(e);
                    tex.textureID = t.textureID;
                    tex.texWidth = t.texWidth;
                    tex.texHeight = t.texHeight;
                    tex.scale = 1;
                }
            }

            if (type == "sprite") {
                if (component["filePath"].is_string()) {
                    std::string path = component["filePath"];
                    path = "Assets/" + path;
                    auto t = LoadTexture(path);
                    auto& sprite = registry->emplace<Sprite>(e);
                    sprite.textureID = t.textureID;
                    sprite.scale = t.scale;
                    sprite.texHeight = t.texHeight;
                    sprite.texWidth = t.texWidth;

                    auto spriteVerts = CreateSpriteVerts();
                    auto& mesh = registry->emplace<Mesh>(e);
                    mesh.arrayID = spriteVerts;
                    mesh.numVerts = 6;

                    auto& rot = registry->emplace<Rotation>(e);
                    rot.rotation = Quaternion::Identity;
                }
            }

            if (type == "ambientLight") {
                if (component["color"].is_array() && component["color"].size() == 3) {
                    auto& alc = registry->emplace<AmbientLightColor>(e);
                    alc.color.x = component["color"][0];
                    alc.color.y = component["color"][1];
                    alc.color.z = component["color"][2];
                }
            }

            if (type == "directionalLight") {
                if (component["direction"].is_array() && component["direction"].size() == 3) {
                    auto& alc = registry->emplace<DirectionalLight>(e);
                    alc.direction.x = component["direction"][0];
                    alc.direction.y = component["direction"][1];
                    alc.direction.z = component["direction"][2];

                    alc.diffuseColor.x = component["diffuseColor"][0];
                    alc.diffuseColor.y = component["diffuseColor"][1];
                    alc.diffuseColor.z = component["diffuseColor"][2];

                    alc.specColor.x = component["specColor"][0];
                    alc.specColor.y = component["specColor"][1];
                    alc.specColor.z = component["specColor"][2];
                }
            }

        }
    }
}



bool LoadOBJMesh(const std::string& path, Mesh& mesh)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<float> tempPositions;
    std::vector<float> tempUVs;
    std::vector<float> tempNormals;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            tempPositions.push_back(x);
            tempPositions.push_back(y);
            tempPositions.push_back(z);
        }
        else if (prefix == "vt") {
            float u, v;
            ss >> u >> v;
            tempUVs.push_back(u);
            tempUVs.push_back(v);
        }
        else if (prefix == "vn") {
            float nx, ny, nz;
            ss >> nx >> ny >> nz;
            tempNormals.push_back(nx);
            tempNormals.push_back(ny);
            tempNormals.push_back(nz);
        }
        else if (prefix == "f") {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char slash;
            for (int i = 0; i < 3; ++i) {
                ss >> vertexIndex[i] >> slash >> uvIndex[i] >> slash >> normalIndex[i];
                vertexIndices.push_back(vertexIndex[i] - 1);
                uvIndices.push_back(uvIndex[i] - 1);
                normalIndices.push_back(normalIndex[i] - 1);
            }
        }
    }

    for (unsigned int i = 0; i < vertexIndices.size(); ++i) {
        vertices.push_back(tempPositions[vertexIndices[i] * 3]);
        vertices.push_back(tempPositions[vertexIndices[i] * 3 + 1]);
        vertices.push_back(tempPositions[vertexIndices[i] * 3 + 2]);

        vertices.push_back(tempNormals[normalIndices[i] * 3]);
        vertices.push_back(tempNormals[normalIndices[i] * 3 + 1]);
        vertices.push_back(tempNormals[normalIndices[i] * 3 + 2]);

        vertices.push_back(tempUVs[uvIndices[i] * 2]);
        vertices.push_back(tempUVs[uvIndices[i] * 2 + 1]);

        indices.push_back(i);
    }
    
    uint32_t vao = UploadMeshToGPU(indices, vertices, 8 * sizeof(float));
    mesh = {
        vao, indices.size()
    };
    return true;
}

std::vector<Mesh> LoadGLTFMeshes(const std::string& path) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = false;
    if (path.ends_with(".glb")) {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
    }
    else {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    }

    if (!warn.empty()) {
        std::cerr << "Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Failed to load glTF file." << std::endl;
        return {};
    }

    std::vector<Mesh> meshes;

    std::vector<uint32_t> m_Indices{};
    std::vector<SkinnedVertex> m_Vertices{};
    std::vector<Submesh> m_Submeshes{};

    for (const auto& primitive : model.meshes[0].primitives) {
        std::vector<float> combinedVertices;
        std::vector<unsigned int> combinedIndices;



        // Access positions
        auto it = primitive.attributes.find("POSITION");
        if (it != primitive.attributes.end()) {
            const tinygltf::Accessor& posAccessor = model.accessors[it->second];
            const tinygltf::BufferView& posBufferView = model.bufferViews[posAccessor.bufferView];
            const tinygltf::Buffer& posBuffer = model.buffers[posBufferView.buffer];

            const unsigned char* posData = posBuffer.data.data() + posBufferView.byteOffset + posAccessor.byteOffset;
            const float* positions = reinterpret_cast<const float*>(posData);

            for (size_t i = 0; i < posAccessor.count; ++i) {
                size_t vertexIndex = i * (posAccessor.ByteStride(posBufferView) / sizeof(float));
                combinedVertices.insert(combinedVertices.end(), positions + vertexIndex, positions + vertexIndex + 3);
            }
        }

        // Access indices
        if (primitive.indices >= 0) {
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

            const unsigned char* indexData = indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset;
            if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                const unsigned short* indices = reinterpret_cast<const unsigned short*>(indexData);
                for (size_t i = 0; i < indexAccessor.count; ++i) {
                    combinedIndices.push_back(indices[i]);
                }
            }
            else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                const unsigned int* indices = reinterpret_cast<const unsigned int*>(indexData);
                for (size_t i = 0; i < indexAccessor.count; ++i) {
                    combinedIndices.push_back(indices[i]);
                }
            }
            else {
                std::cerr << "Unsupported index type" << std::endl;
                continue;
            }
        }

        uint32_t vao = UploadMeshToGPU(combinedIndices, combinedVertices, 3 * sizeof(float));
        meshes.push_back({ vao, combinedIndices.size() });
    }

    return meshes;
}
