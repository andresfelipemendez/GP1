#pragma once

#include <vector>
#include <SDL.h>

class Game
{
public:
	Game();
	~Game();

	bool Initialize();
	void RunLoop();
	void ShutDown();

	void AddActor(class Actor* actor);
private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	bool mIsRunning = false;
	bool mUpdatingActors = false;

	Uint32 mTicksCount = 0;

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};
