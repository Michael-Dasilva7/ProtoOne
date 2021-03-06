#ifndef EFFECT_H_
#define EFFECT_H_

#include "Animation.h"

// a class for temporary animated effects
class Effect
{
    Animation                   mAnim;

    Vec2                        mCenterPos; // position of sprite center in world coordinates

public:
                                Effect(SDL_Texture* tex, int numCells, float duration, bool loopable, const Vec2& pos);

    void                        Update(float dt)    { mAnim.AddTime(dt); }

    bool                        IsDone() const      { return mAnim.IsDone(); }

    void                        Draw(SDL_Renderer* renderer, Camera* camera) const;
};

#endif
