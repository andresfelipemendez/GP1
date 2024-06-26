#include "Game.h"
#include <SDL_image.h>
#include "Components.h"
#include "Math.h"
#include <SDL.h>
#include "Systems.h"
#include "Random.h"

bool Initialize(GameData* gd, entt::registry* registry)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	gd->window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 2)",
		100,
		100,
		1024,
		768,
		0
	);
	if (!gd->window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	gd->renderer = SDL_CreateRenderer(
		gd->window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!gd->renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	Random::Init();

	LoadData(gd, registry);

	gd->ticksCount = SDL_GetTicks();
	gd->isRunning = true;
	return true;
}
void RunLoop(GameData* gd, entt::registry* registry)
{
	while (gd->isRunning) {
		ProcessInput(gd, registry);
		UpdateGame(gd, registry);
		GenerateOutput(gd, registry);
	}
}

void ShutDown(GameData* gd)
{
}

void ProcessInput(GameData* gd, entt::registry* registry)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gd->isRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		gd->isRunning = false;
	}

	InputSystem(registry, state);
	ShootingSystem(registry, gd->renderer, state);
}

void UpdateGame(GameData* gd, entt::registry* registry)
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), gd->ticksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - gd->ticksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	gd->ticksCount = SDL_GetTicks();

	MovementSystem(registry, deltaTime);
	CollisionSystem(registry, deltaTime);
}

void GenerateOutput(GameData* gd, entt::registry* registry)
{
	SDL_SetRenderDrawColor(gd->renderer, 150, 150, 150, 255);
	SDL_RenderClear(gd->renderer);

	RenderSystem(gd->renderer, registry);

	SDL_RenderPresent(gd->renderer);
}

std::unordered_map<std::string, SDL_Texture*> mTextures;
SDL_Texture* GetTexture(const std::string& fileName, SDL_Renderer* renderer)
{
	SDL_Texture* tex = nullptr;

	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void LoadData(GameData* gd, entt::registry* registry)
{
	auto ship = registry->create();
	registry->emplace<Transform>(ship, 100.0f, 384.0f);
	SDL_Texture* tex = GetTexture("Assets/Ship.png", gd->renderer);
	int width, height;
	SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);

	registry->emplace<Move>(ship, 0.0f, 0.0f);
	registry->emplace<Sprite>(ship, tex, 100, width, height, 1.0f);

	auto& inp = registry->emplace<Input>(ship);
	inp.maxFwdSpeed = 300.0f;
	inp.maxAngSpeed = Math::TwoPi;
	inp.forwardKey = SDL_SCANCODE_W;
	inp.backKey = SDL_SCANCODE_S;
	inp.clockwiseKey = SDL_SCANCODE_A;
	inp.counterClockwiseKey = SDL_SCANCODE_D;

	registry->emplace<Shoot>(ship, SDL_SCANCODE_SPACE, 0.5f);

	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++) {

		auto asteroid = registry->create();
		SDL_Texture* tex = GetTexture("Assets/Asteroid.png", gd->renderer);
		int width, height;
		SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);

		registry->emplace<Sprite>(asteroid, tex, 100, width, height, 1.0f);

		Vector2 randPos = Random::GetVector(
			Vector2::Zero,
			Vector2(1024.0f, 768.0f)
		);

		auto& pos = registry->emplace<Transform>(asteroid);
		pos.x = randPos.x;
		pos.y = randPos.y;
		pos.rot = Random::GetFloatRange(0.0f, Math::TwoPi);

		registry->emplace<Move>(asteroid, 0.0f, 150.0f);

		registry->emplace<Circle>(asteroid, 40.0f);
	}

}

void UnloadData(GameData* gd)
{

}

