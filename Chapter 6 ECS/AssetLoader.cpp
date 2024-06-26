#include "AssetLoader.h"
#include "Components.h"
#include <entt/entt.hpp>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <toml.h>
#include <SDL.h>

#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


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
                auto near = get_json_value<float>(component, "near");
                auto far = get_json_value<float>(component, "far");

                if (fov && near && far) {
                    auto& cam = registry->emplace<Camera>(e);
                    cam.fov = *fov;
                    cam.near = *near;
                    cam.far = *far;
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
					rot.rotation = Quaternion::FromEuler(rotEuler);
				}
				else {
					std::cerr << "Rotation component skipped due to missing data." << std::endl;
                }
            }

			if (type == "mesh") {
                if (component["path"].is_string()) {
                    auto& mesh = registry->emplace<Mesh>(e);
					LoadMesh(component["path"]);
				}
				else {
					std::cerr << "Mesh component skipped due to missing data." << std::endl;
				}
				/*mesh.path = component["path"];*/
			}
        }
    }
}

void LoadMesh(const std::string& path) {
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
		return;
	}

	if (model.meshes.empty()) {
		std::cerr << "No meshes found in glTF file." << std::endl;
		return;
	}

	auto& mesh = model.meshes[0];
	if (mesh.primitives.empty()) {
		std::cerr << "No primitives found in mesh." << std::endl;
		return;
	}

	auto& primitive = mesh.primitives[0];
	if (primitive.attributes.empty()) {
		std::cerr << "No attributes found in primitive." << std::endl;
		return;
	}

	auto& attribs = primitive.attributes;
	if (attribs.find("POSITION") == attribs.end()) {
		std::cerr << "No position attribute found in primitive." << std::endl;
		return;
	}

	auto& accessor = model.accessors[attribs["POSITION"]];
	auto& bufferView = model.bufferViews[accessor.bufferView];
	auto& buffer = model.buffers[bufferView.buffer];

	if (bufferView.byteStride == 0) {
		std::cerr << "No byte stride found in buffer view." << std::endl;
		return;
	}

	if (bufferView.byteOffset == 0) {
		std::cerr << "No byte offset found in buffer view." << std::endl;
		return;
	}

	if (bufferView.byteLength == 0) {
		std::cerr << "No byte length found in buffer view." << std::endl;
		return;
	}

	if (buffer.data.empty()) {
		std::cerr << "No data found in buffer." << std::endl;
		return;
	}

    if (accessor.count == 0) {
        std::cerr << "No count found in accessor." << std::endl;
        return;
    }
}