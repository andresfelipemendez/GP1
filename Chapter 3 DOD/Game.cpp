#include "Game.h"
#include <SDL_image.h>
#include "Components.h"
#include "Math.h"
#include <SDL.h>
#include "Systems.h"
#include "Random.h"
#include <unordered_map>

bool InitializeGame(GameData& gd, Registry& r)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	gd.window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 2)",
		100,
		100,
		1024,
		768,
		0
	);
	if (!gd.window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	gd.renderer = SDL_CreateRenderer(
		gd.window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!gd.renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	Random::Init();

	LoadData(gd, r);

	gd.ticksCount = SDL_GetTicks();
	gd.isRunning = true;
	return true;
}
void RunLoop(GameData& gd, Registry& r)
{
	while (gd.isRunning) {
		Input(			gd, r		);
		Update(			gd, r		);
		GenerateOutput(			gd, r		);
	}
}

void ShutDown(GameData* gd)
{
}

void Input(
	GameData& gd, 
	Registry& r
)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gd.isRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		gd.isRunning = false;
	}

	ProcessInput(r, state);
	//ShootingSystem(registry, gd->renderer, state);
}

void Update(GameData& gd, Registry& r)
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), gd.ticksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - gd.ticksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	gd.ticksCount = SDL_GetTicks();

	UpdateGame(r, deltaTime);

	CollisionSystem(r, deltaTime);
}

void GenerateOutput(GameData& gd, Registry& r)
{
	SDL_SetRenderDrawColor(gd.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gd.renderer);

	RenderGame(gd.renderer, r);

	SDL_RenderPresent(gd.renderer);
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

void LoadData(GameData& gd, Registry& r)
{
	SDL_Texture* shipTex = GetTexture("Assets/Ship.png", gd.renderer);
	int width, height;
	SDL_QueryTexture(shipTex, nullptr, nullptr, &width, &height);

	r.transformData.pos.push_back({ 100.0f,384.0f });
	r.transformData.rot.push_back(0.0f);

	SpriteData& spriteData = r.sprites;
	spriteData.textures.push_back(shipTex);
	spriteData.texWidths.push_back(width);
	spriteData.texHeights.push_back(height);


	r.moveData.angularSpeed.push_back(0.0f);
	r.moveData.forwardSpeed.push_back(0.0f);

	r.inputData.forwardKey = SDL_SCANCODE_W;
	r.inputData.backKey = SDL_SCANCODE_S;
	r.inputData.clockwiseKey = SDL_SCANCODE_A;
	r.inputData.counterClockwiseKey = SDL_SCANCODE_D;
	r.inputData.maxFwdSpeed = 300.0f;
	r.inputData.maxAngSpeed = Math::TwoPi;

	r.shootData.shootKey = SDL_SCANCODE_SPACE;
	r.shootData.shootCooldown = 0.5f;

	r.entityIndices.shipIndex = 0;

	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; ++i) {
		SDL_Texture* asteroidTex = GetTexture("Assets/Asteroid.png", gd.renderer);
		SDL_QueryTexture(asteroidTex, nullptr, nullptr, &width, &height);

		spriteData.textures.push_back(asteroidTex);
		spriteData.texWidths.push_back(width);
		spriteData.texHeights.push_back(height);

		Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
		r.transformData.pos.push_back({ randPos.x,randPos.y });
		r.transformData.rot.push_back(Random::GetFloatRange(0.0f, Math::TwoPi));

		r.moveData.angularSpeed.push_back(0.0f);
		r.moveData.forwardSpeed.push_back(150.0f);

		r.circleData.radius.push_back(40.0f);

		r.entityIndices.asteroidIndices.push_back(i + 1); // Ship is at 0, asteroids start at 1
	}
}

void UnloadData(GameData* gd)
{

}

