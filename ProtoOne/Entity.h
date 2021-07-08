#ifndef ENTITY_H_
#define ENTITY_H_

#include <SDL.h>
#include "Math.h"
#include "ColorChange.h"

//forward declarations
class Camera;
class Animation;

class Entity
{
protected:
	
    SDL_Texture*            mTex;
    int                     mTexWidth;
    int                     mTexHeight;
    float                   mSpriteAngleOffset; // correction for sprites facing in different directions

    Vec2                    mCenterPos; // position of sprite center in world coordinates
    float                   mAngle;
    float                   mRadius;
    float                   mHealth;	
	float					mHoverPoint;

	ColorChange* 			mColorChange;
	unsigned short int      mLayer;
	

public:
	Vec2					mVelocity;
	//forward declaring class Animation to prevent circular dependency.
	// therefore i need to declare animation as a pointer because its just a header
	// and we know there will be a class as animation included in the cpp and will be there in the declaration
	
	Animation*              mCurrentAnimation;
    int                     mRefCount;

							Entity(SDL_Texture* tex);
							Entity(SDL_Texture* tex, int numCells, float duration, bool loopable);
							//Entity::Entity(SDL_Texture* tex, int numCells, float duration, const Vec2& pos)

    virtual                 ~Entity();

                            // need to call this unless sprite faces right in texture
    virtual void            SetSpriteAngleCorrection(CardinalDir spriteForwardDir);

    virtual const Vec2&     Center() const      { return mCenterPos; }

    virtual void            SetCenter(const Vec2& pos)  { mCenterPos = pos; }
    virtual void            SetCenter(float x, float y) { mCenterPos = Vec2(x, y); }

    virtual float           Radius() const      { return mRadius; }

    virtual float           Health() const      { return mHealth; }
    virtual void            SetHealth(float h)  { mHealth = h; }

    virtual bool            IsDead() const      { return mHealth <= 0; }
    virtual bool            IsAlive() const     { return mHealth > 0; }

	virtual float			Left() const		{ return mCenterPos.x - mRadius; }
	virtual float			Right() const		{ return mCenterPos.x + mRadius; }
	virtual float			Top() const			{ return mCenterPos.y - mRadius; }
	virtual float			Bottom() const		{ return mCenterPos.y + mRadius; }

    virtual void            SetLeft(float x)    { mCenterPos.x = x + mRadius; }
    virtual void            SetRight(float x)   { mCenterPos.x = x - mRadius; }
    virtual void            SetTop(float y)     { mCenterPos.y = y + mRadius; }
    virtual void            SetBottom(float y)  { mCenterPos.y = y - mRadius; }

    virtual float           Angle() const       { return mAngle; }
    virtual void            SetAngle(float deg) { mAngle = deg; }

	virtual unsigned short int      Layer() const { return mLayer; }
	virtual void			SetLayer(unsigned short int layer) { mLayer = layer; }

	virtual void            SetLAyer(float deg) { mAngle = deg; }

    virtual void            SetOrientation(CardinalDir dir) { this->SetAngle(CardinalAngle[dir]); }

    virtual Vec2            ForwardDir() const;
    virtual void            SetForwardDir(const Vec2& dir);
    virtual void            SetForwardDir(float dx, float dy)       { SetForwardDir(Vec2(dx, dy)); }

	virtual void		    Update(float dt)    = 0;

	virtual void			Draw(SDL_Renderer* renderer, Camera* camera, SDL_RendererFlip flipType = SDL_FLIP_NONE) const;

	virtual void			setAnimation(SDL_Texture* tex, int numCells, float duration, bool loopable, SDL_RendererFlip flip = SDL_FLIP_NONE);
	virtual void			setAnimationTexture(SDL_Texture* tex);

	virtual void	        addTimeToAnimation(float dt);


};


inline void Entity::SetSpriteAngleCorrection(CardinalDir spriteForwardDir)
{
    mSpriteAngleOffset = CardinalAngle[spriteForwardDir];
}

inline Vec2 Entity::ForwardDir() const
{
    float radAngle = Deg2Rad(mAngle);
    return Vec2(std::cos(radAngle), std::sin(radAngle));
}

inline void Entity::SetForwardDir(const Vec2& dir)
{
    mAngle = Rad2Deg(std::atan2(dir.y, dir.x));
}

#endif
