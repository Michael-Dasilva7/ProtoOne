#include <iostream>

#include "Missile.h"

Missile::Missile(SDL_Texture* tex, Entity* shooter)
	: Entity(tex)
    , mSpeed(400)   // in pixels per second
    , mShooter(shooter)
	

{
	SetLayer(2);
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
    mCenterPos += mSpeed * dt * ForwardDir();
}
