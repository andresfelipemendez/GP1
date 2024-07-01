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
                    if (path.ends_with(".glft") || path.ends_with(".glb")) {
                       vertexArray = LoadGLTFMesh(path);
                    }
                    
                    if (path.ends_with(".obj") && !LoadOBJMesh(path, vertexArray)) {
                        continue;
                    }
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

Mesh LoadGLTFMesh(const std::string& path) {
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
   
  
    if (model.scenes.size() > 1) {
        SDL_Log("the engine don't support more than one scene");
        return {};
    }
    const tinygltf::Scene& scene = model.scenes[0];
    if (scene.nodes.size() > 1) {
        SDL_Log("the engine don't support more than one node per gltf file");
        return {};
    }
    if (model.meshes.size() > 1) {
        SDL_Log("the engine don't support more than one mesh per gltf file");
        return {};
    }
    const tinygltf::Mesh& mesh = model.meshes[0];
    
    auto size = mesh.primitives.size();

    GLuint vao;
    int numVertices = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    std::map<int, GLuint> vbos;
    for (size_t i = 0; i < model.bufferViews.size(); ++i) {
        const tinygltf::BufferView& bufferView = model.bufferViews[i];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos[i] = vbo;
        if (bufferView.target == GL_ELEMENT_ARRAY_BUFFER) {
            numVertices = bufferView.byteLength;
        }
        glBindBuffer(bufferView.target, vbo);
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

    }


    const tinygltf::Primitive& primitive = mesh.primitives[0];
    const tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
    

    for (const auto& attrib : primitive.attributes) {
        const tinygltf::Accessor accessor = model.accessors[attrib.second];
        int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

        int size = 1;
        if (accessor.type != TINYGLTF_TYPE_SCALAR) {
            size = accessor.type;
        }

        int vaa = -1;
        if (attrib.first.compare("POSITION") == 0) vaa = 0;
        if (attrib.first.compare("NORMAL") == 0) vaa = 1;
        if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
        if (vaa > -1) {
            glEnableVertexAttribArray(vaa);
            glVertexAttribPointer(vaa, size, accessor.componentType,
                accessor.normalized ? GL_TRUE : GL_FALSE,
                byteStride, BUFFER_OFFSET(accessor.byteOffset));
        }
        else {
            std::cout << "vaa missing: " << attrib.first << std::endl;
        }
    }

    Mesh m;
    m.arrayID = vao;
    m.numVerts = indexAccessor.count;
    return m;
}