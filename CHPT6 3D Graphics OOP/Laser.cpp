#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game) : Actor(game), mDeathTimer(1.0f)
{
  SpriteComponent* sc = new SpriteComponent(this);
  sc->SetTexture(game->GetTexture("Assets/Laser.png"));

  MoveComponent* mc = new MoveComponent(this);
  mc->mForwardSpeed = 800.0f; 

  mCircle = new CircleComponent(this);
  mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime)
{
  mDeathTimer -= deltaTime;
  if(mDeathTimer <= 0.0f){
    mState = EDead;
  }
  else {
    for(auto ast : mGame->GetAsteroids()) {
      if(Intersect(*mCircle, *(ast->mCircle)))
      {
        mState = EDead;
        ast->mState = EDead;
        break;
      }
    }
  }
}
