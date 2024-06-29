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

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Renderer.h"

ImageFile::ImageFile(const std::string& fileName)
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
    if (!data) {
        SDL_Log("STB failed to load image %s: %s", fileName.c_str(),
            stbi_failure_reason());
    }
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

			if (type == "mesh") {
                if (component["filePath"].is_string()) {
                    std::string path = component["filePath"];
                    path = "Assets/" + path;
                    Mesh vertexArray = LoadMesh(path);
                    auto& mesh = registry->emplace<Mesh>(e);
                    mesh.arrayID = vertexArray.arrayID;
                    mesh.numVerts = vertexArray.numVerts;
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
        }
    }
}


Mesh LoadMesh(const std::string& path) {
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

    for (const auto& mesh : model.meshes) {
        for (const auto& primitive : mesh.primitives) {            
            const tinygltf::Accessor& indexAccesor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccesor.bufferView];
            const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

            std::vector<uint32_t> indices(indexAccesor.count);
            std::vector<float> vertices;

            memcpy(indices.data(), &indexBuffer.data[indexBufferView.byteOffset + indexAccesor.byteOffset],
                indexAccesor.count * sizeof(uint32_t));

            // Determine the stride and offset for each attribute
            std::unordered_map<std::string, size_t> attributeOffsets;
            uint32_t stride = 0;

            for (const auto& attrib : primitive.attributes) {
                const tinygltf::Accessor& accessor = model.accessors[attrib.second];
                attributeOffsets[attrib.first] = stride;
                stride += accessor.type * sizeof(float);
            }

            // Allocate vertex buffer
            unsigned int vertexCount = model.accessors[primitive.attributes.at("POSITION")].count;
            vertices.resize(vertexCount * stride);

            // Copy attribute data to vertex buffer
            for (const auto& attrib : primitive.attributes) {
                const tinygltf::Accessor& accessor = model.accessors[attrib.second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                size_t elementSize = accessor.type * sizeof(float);
                size_t offset = attributeOffsets[attrib.first];

                for (size_t i = 0; i < vertexCount; ++i) {
                    memcpy(&vertices[i * stride + offset],
                        &buffer.data[bufferView.byteOffset + accessor.byteOffset + i * elementSize],
                        elementSize);
                }
            }

            if (stride == 32) {
                return {
                    UploadMeshToGPU(indices, vertices, stride),
                    vertexCount
                };
            }
        }
    }


}