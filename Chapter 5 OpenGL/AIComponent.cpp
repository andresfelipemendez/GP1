#include "AIComponent.h"
#include "AIState.h"
#include <SDL.h>

AIComponent::AIComponent(Actor* owner) : Component(owner)
{
}

void AIComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChageState(const std::string& name)
{
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		mCurrentState->OnEnter();
	}
	else {
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::ReigsterState(AIState* state)
{
	mStateMap.emplace(state->GetName(), state);
}
