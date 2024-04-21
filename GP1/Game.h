#include <SDL.h>

struct Vector2 {
	float x = 0;
	float y = 0;
};


struct Paddle {
	Vector2 position{ 10.0f, 100.0f };
	int height = 100;
	int thickness = 15;
	int direction = 0;
};

struct Ball
{
	Vector2 position{ 100.0f, 100.0f };
	Vector2 velocity{ -200.0f, 235.0f };
	int thickness = 15;
};

struct Wall {
	int thickness{ 15 };
};

struct GameData {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool isRunning = false;
	Uint32 ticksCount = 0;

	Paddle paddle;
	Ball ball;
	Wall wall;
};

class Game {
public:
  Game ();
  ~Game ();
  bool Initialize(GameData& gameData);
  void RunLoop(GameData& gameData);
  void Shutdown(GameData& gameData);

private:
  void ProcessInput(GameData& gameData);
  void UpdateGame(GameData& gameData);
  void GenerateOutput(GameData& gameData);
};
