#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL.h>
#include "Math.h"
#include "Entity.h"

class Camera
{
    float                       mWorldWidth;
    float                       mWorldHeight;

    float                       mViewWidth;
    float                       mViewHeight;

    float                       mViewLeft;
    float                       mViewTop;

    Entity*                     mTarget;

public:
                                Camera(float viewWidth, float viewHeight, float worldWidth, float worldHeight);

    void                        LookAt(const Vec2& wpos);

    float                       ViewWidth() const       { return mViewWidth; }
    float                       ViewHeight() const      { return mViewHeight; }

    float                       ViewLeft() const        { return mViewLeft; }
    float                       ViewRight() const       { return mViewLeft + mViewWidth; }
    float                       ViewTop() const         { return mViewTop; }
    float                       ViewBottom() const      { return mViewTop + mViewHeight; }

    //
    // coordinate conversions
    //
    float                       ScreenToWorldX(int scrx) const;
    float                       ScreenToWorldY(int scry) const;

    int                         WorldToScreenX(float wx) const;
    int                         WorldToScreenY(float wy) const;

    int                         WorldToScreenXD(float wx, float depth) const;
    int                         WorldToScreenYD(float wy, float depth) const;

    Vec2                        ScreenToWorld(int scrx, int scry) const;
    SDL_Point                   WorldToScreen(float wx, float wy) const;

    Vec2                        ScreenToWorld(const SDL_Point& scrpos) const;
    SDL_Point                   WorldToScreen(const Vec2& wpos) const;

    void                        SetTarget(Entity* target);

    void                        Update(float dt);
};


//
// inlinable implementations of some short methods
//

inline float Camera::ScreenToWorldX(int scrx) const
{
    return scrx + mViewLeft;
}

inline float Camera::ScreenToWorldY(int scry) const
{
    return scry + mViewTop;
}

inline int Camera::WorldToScreenX(float wx) const
{
    return RoundToInt(wx - mViewLeft);
}

inline int Camera::WorldToScreenY(float wy) const
{
    return RoundToInt(wy - mViewTop);
}

inline int Camera::WorldToScreenXD(float wx, float depth) const
{
    float f = 1.0f - depth;
    return RoundToInt(wx - mViewLeft * f);
}

inline int Camera::WorldToScreenYD(float wy, float depth) const
{
    float f = 1.0f - depth;
    return RoundToInt(wy - mViewTop * f);
}

inline Vec2 Camera::ScreenToWorld(int scrx, int scry) const
{
    Vec2 wpos;
    wpos.x = ScreenToWorldX(scrx);
    wpos.y = ScreenToWorldY(scry);
    return wpos;
}

inline SDL_Point Camera::WorldToScreen(float wx, float wy) const
{
    SDL_Point scrpos;
    scrpos.x = WorldToScreenX(wx);
    scrpos.y = WorldToScreenY(wy);
    return scrpos;
}

inline Vec2 Camera::ScreenToWorld(const SDL_Point& scrpos) const
{
    return ScreenToWorld(scrpos.x, scrpos.y);
}

inline SDL_Point Camera::WorldToScreen(const Vec2& wpos) const
{
    return WorldToScreen(wpos.x, wpos.y);
}


#endif
