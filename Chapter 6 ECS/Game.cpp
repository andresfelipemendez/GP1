#include "Game.h"
#include "Components.h"
#include "GL/glew.h"
#include "Math.h"
#include "Random.h"
#include "Rendering.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "Systems.h"
#include <SDL.h>
#include <SDL_image.h>
#include "AssetLoader.h"

bool Initialize(GameData *gd, entt::registry *registry) {


  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  gd->window = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
                                1024, 768, SDL_WINDOW_OPENGL);

  if (!gd->window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  gd->context = SDL_GL_CreateContext(gd->window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    SDL_Log("Failed to initialize GLEW.");
    return false;
  }

  CreateSpriteVerts(gd, registry);

  Random::Init();

  LoadScene(registry, "Assets/scene.json");
 
  gd->ticksCount = SDL_GetTicks();
  gd->isRunning = true;
  return true;
}

void CreateSpriteVerts(GameData *gd, entt::registry *registry) {
  // clang-format off
  std::vector<float> vertices {
     -0.5f,   0.5f,  0.f,  0.f,  0.f,
      0.5f,   0.5f,  0.f,  1.f,  0.f,
      0.5f,  -0.5f,  0.f,  1.f,  1.f,
     -0.5f,  -0.5f,  0.f,  0.f,  1.f
  };

  std::vector<unsigned int> indices {
    0,1,2,
    2,3,0
  };

  LoadMesh(registry, vertices, indices);
  //clang-format on
}

void RunLoop(GameData *gd, entt::registry *registry) {
  while (gd->isRunning) {
    ProcessInput(gd, registry);
    UpdateGame(gd, registry);
    GenerateOutput(gd, registry);
  }
}

void ShutDown(GameData *gd) {}

void ProcessInput(GameData *gd, entt::registry *registry) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      gd->isRunning = false;
      break;
    default:
      break;
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_ESCAPE]) {
    gd->isRunning = false;
  }

  InputSystem(registry, state);
  ShootingSystem(registry, state);
}

void UpdateGame(GameData *gd, entt::registry *registry) {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), gd->ticksCount + 16))
    ;

  float deltaTime = (SDL_GetTicks() - gd->ticksCount) / 1000.0f;
  if (deltaTime > 0.05f) {
    deltaTime = 0.05f;
  }
  gd->ticksCount = SDL_GetTicks();

  MovementSystem(registry, deltaTime);
  CollisionSystem(registry, deltaTime);
}

void GenerateOutput(GameData *gd, entt::registry *registry) {
  RenderSystem(gd, registry);
}

std::unordered_map<std::string, Texture> mTextures;
Texture GetTexture(const std::string &fileName) {
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end()) {
	    return iter->second;
    } 
	auto tex = LoadTexture(fileName);
    mTextures.emplace(fileName, tex);
    return tex;
}


std::unordered_map<std::string, Shader> mShaders;
Shader GetShader(const std::string& vertexShader, const std::string& fragmentShader) {
	auto iter = mShaders.find(vertexShader+fragmentShader);
	if (iter != mShaders.end()) {
		return iter->second;
	}
	auto shader = LoadShader(vertexShader, fragmentShader);
	mShaders.emplace(vertexShader + fragmentShader, shader);
	return shader;

}

void LoadData(GameData *gd, entt::registry *registry) {
	LoadScene(registry, "Assets/scene.json");
}

void UnloadData(GameData *gd) {}
