#include "Effect.h"

Effect::Effect(SDL_Texture* tex, int numCells, float duration, const Vec2& pos)
    : mAnim(tex, numCells, duration, false)
    , mCenterPos(pos)
{
}

void Effect::Draw(SDL_Renderer* renderer, Camera* camera) const
{
    mAnim.Draw(renderer, mCenterPos, camera);
}
