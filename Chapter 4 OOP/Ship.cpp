#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game) :
	Actor(game),
	mLaserCooldown(0.0f)
{
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
	 
	InputComponent* ic = new InputComponent(this);
	ic->mForwardKey = SDL_SCANCODE_W;
	ic->mBackKey = SDL_SCANCODE_S;
	ic->mClockwiseKey = SDL_SCANCODE_A;
	ic->mCounterClockwiseKey = SDL_SCANCODE_D;
	ic->mMaxForwardSpeed = 300.0f;
	ic->mMaxAngularSpeed = Math::TwoPi;
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

}

void Ship::ActorInput(const uint8_t* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f) {
		Laser* laser = new Laser(mGame);
		laser->mPosition = mPosition;
		laser->mRotation = mRotation;

		mLaserCooldown = 0.5f;
	}
}