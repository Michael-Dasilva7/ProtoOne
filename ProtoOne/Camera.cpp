#include "Camera.h"

Camera::Camera(float viewWidth, float viewHeight, float worldWidth, float worldHeight)
    : mWorldWidth(worldWidth)
    , mWorldHeight(worldHeight)
    , mViewWidth(viewWidth)
    , mViewHeight(viewHeight)
    , mViewLeft(0.5f * worldWidth - 0.5f * viewWidth)
    , mViewTop(0.5f * worldHeight - 0.5f * viewHeight)
    , mTarget(NULL)
{
}

void Camera::LookAt(const Vec2& wpos)
{
    //// center on the given position
    //mViewLeft = (wpos.x - 0.5f * mViewWidth);
    //mViewTop = (wpos.y - 0.5f * mViewHeight);
	mViewLeft = (wpos.x - 0.5f * mViewWidth);
	mViewTop = (wpos.y - 0.5f * mViewHeight);
    // keep camera viewing offset in valid range
    if (mViewLeft < 0) {
        mViewLeft = 0;
    }
    if (mViewTop < 0) {
        mViewTop = 0;
    }
    if (mViewLeft + mViewWidth > mWorldWidth) {
        mViewLeft = mWorldWidth - mViewWidth;
    }
    if (mViewTop + mViewHeight > mWorldHeight) {
        mViewTop = mWorldHeight - mViewHeight;
    }
}

void Camera::SetTarget(Entity* target)
{
    mTarget = target;
    if (target) {
        LookAt(target->Center());
    }
}

void Camera::Update(float dt)
{
    if (mTarget) {
        LookAt(mTarget->Center());
    }
}
