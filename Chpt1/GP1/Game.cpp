#include "Game.h"

Game::Game(){
	
}

Game::~Game() {

}

bool Game::Initialize(GameData& gameData) {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	gameData.window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)",
		100,
		100,
		1024,
		768,
	0
	);

	if (!gameData.window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	gameData.renderer = SDL_CreateRenderer(
		gameData.window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	gameData.isRunning = true;
	return true;
}

void Game::RunLoop(GameData& gameData) {
	while (gameData.isRunning)
	{
		ProcessInput(gameData);
		UpdateGame(gameData);
		GenerateOutput(gameData);
	}
}

void Game::Shutdown(GameData& gameData) {
	SDL_DestroyRenderer(gameData.renderer);
	SDL_DestroyWindow(gameData.window);
	SDL_Quit();
}

void Game::ProcessInput(GameData& gameData) {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameData.isRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		gameData.isRunning = false;
	}

	gameData.paddle.direction = 0;
	if (state[SDL_SCANCODE_W])
	{
		gameData.paddle.direction -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		gameData.paddle.direction += 1;
	}

}

void Game::UpdateGame(GameData& gameData) {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), gameData.ticksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - gameData.ticksCount) / 1000.0f;
	gameData.ticksCount = SDL_GetTicks();

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	Paddle& paddle = gameData.paddle;
	if (gameData.paddle.direction != 0)
	{
		gameData.paddle.position.y += gameData.paddle.direction * 300.0f * deltaTime;

		if(gameData.paddle.position.y < (gameData.paddle.height /2.0f + gameData.paddle.thickness))
		{
			gameData.paddle.position.y = (gameData.paddle.height / 2.0f) + gameData.paddle.thickness;
		} 
		else if(gameData.paddle.position.y > (768.0f - gameData.paddle.height /2.0f - gameData.paddle.thickness))
		{
			gameData.paddle.position.y = ((768.0f - gameData.paddle.height / 2.0f) - gameData.paddle.thickness);
		}
	}

	Ball& ball = gameData.ball;
	ball.position.x += ball.velocity.x * deltaTime;
	ball.position.y += ball.velocity.y * deltaTime;
	
	

	if ((ball.position.y <= ball.thickness && ball.velocity.y < 0) ||
		(ball.position.y >= 768 && ball.velocity.y > 0)) 
	{
		ball.velocity.y *= -1;
	}

	if ((ball.position.x <= 0 && ball.velocity.x < 0) ||
		(ball.position.x >= 1024 && ball.velocity.x > 0))
	{
		ball.velocity.x *= -1;
	}

	float diff = SDL_fabsf(paddle.position.y - ball.position.y);
	if (
		(diff <= (paddle.height / 2.0f)) &&
		(ball.position.x <= 25.0f && ball.position.x >= 20.0f) &&
		(ball.velocity.x < 0)
	)
	{
		ball.velocity.x *= -1;
	}

}

void Game::GenerateOutput(GameData& gameData) {
	SDL_SetRenderDrawColor(
		gameData.renderer, 0, 0, 255, 255
	);
	SDL_RenderClear(gameData.renderer);

	SDL_SetRenderDrawColor(gameData.renderer, 255, 255, 255, 255);

	SDL_Rect wall{
		0,0,1024,gameData.wall.thickness
	};

	SDL_Rect paddle{
		static_cast<int>(gameData.paddle.position.x - gameData.paddle.thickness / 2),
		static_cast<int>(gameData.paddle.position.y - gameData.paddle.thickness / 2),
		gameData.paddle.thickness,
		gameData.paddle.height
	};

	SDL_Rect ball{
		static_cast<int>(gameData.ball.position.x - gameData.ball.thickness / 2),
		static_cast<int>(gameData.ball.position.y - gameData.ball.thickness / 2),
		gameData.ball.thickness,
		gameData.ball.thickness
	};

	SDL_RenderFillRect(gameData.renderer, &wall);
	SDL_RenderFillRect(gameData.renderer, &paddle);
	SDL_RenderFillRect(gameData.renderer, &ball);

	SDL_RenderPresent(gameData.renderer);
}


