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

#define _CRT_SECURE_NO_WARNINGS
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

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
        }
    }
}


void LoadMesh() {
	cgltf_options options = { 0 };
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, "scene.gltf", &data);
	if (result == cgltf_result_success)
	{
		/* TODO make awesome stuff */
		cgltf_free(data);
	}
	else
	{
		/* TODO error handling */
	}
}