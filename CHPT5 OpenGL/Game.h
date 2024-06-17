#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void ShutDown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);


	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

	//private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void LoadData();
	void UnloadData();
	void CreateSpriteVerts();

	class Shader* mSpriteShader = nullptr;
	class VertexArray* mSpriteVerts = nullptr;

	std::unordered_map<std::string, SDL_Texture*> mTextures;

	std::vector<class Actor*> mActors;

	std::vector<class Actor*> mPendingActors;

	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow = nullptr;
	SDL_GLContext mContext;
	Uint32 mTicksCount = 0;
	bool mIsRunning = false;
	bool mUpdatingActors = false;
	
	class Ship* mShip = nullptr;
	std::vector<class Asteroid*> mAsteroids;

};
