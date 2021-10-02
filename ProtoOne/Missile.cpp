#include <iostream>

#include "Missile.h"

Missile::Missile(SDL_Texture* tex, Entity* shooter, int numCells, float duration, bool loopable)
	: Entity(tex,numCells,duration,loopable)
    , mSpeed(400)   // in pixels per second
    , mShooter(shooter)
	
{
	SetLayer(2.0f);
    if (mShooter) {
        ++mShooter->mRefCount;
    }
}

Missile::~Missile()
{
    std::cout << "Missile destroyed" << std::endl;

    if (mShooter) {
        --mShooter->mRefCount;
    }
}

void Missile::SetSpeed(float speed)
{
    mSpeed = speed;
}

void Missile::Update(float dt)
{
    // move along the forward direction
	this->addTimeToAnimation(dt);
    mCenterPos += mSpeed * dt * ForwardDir();
}
