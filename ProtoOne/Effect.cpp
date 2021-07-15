#include "Effect.h"

Effect::Effect(SDL_Texture* tex, int numCells, float duration,bool loopable, const Vec2& pos)
    : mAnim(tex, numCells, duration, loopable)
    , mCenterPos(pos)
{
}

void Effect::Draw(SDL_Renderer* renderer, Camera* camera) const
{
    mAnim.Draw(renderer, mCenterPos, camera);
}
