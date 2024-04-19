#include <SDL.h>

struct Vector2 {
	float x;
	float y;
};

class Game {
public:
  Game ();
  ~Game ();
  bool Initialize();
  void RunLoop();
  void Shutdown();

private:
  const int thickness = 15;
  const int paddleH = 100;
  int mPaddelDir = 0;
  Vector2 mPaddlePos;
  Vector2 mBallPos;

  SDL_Window* mWindow = nullptr;
  SDL_Renderer* mRenderer = nullptr;
  bool mIsRunning = false;
  Uint32 mTicksCount = 0;

  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
};
