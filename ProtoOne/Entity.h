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
	enum  mState {
		IDLE,
		DAMAGED,
		ATTACKING,
		RUNNING,
		WALKING,
		STANDING
	};
	
	mState mCurrentState;
	mState mPreviousState;

	SDL_Rect*				mHitbox;

	bool					mInCutscene;
	bool					mActionable;

	Vec2					mVelocity;
	Vec2					mPreviousPosition;
	//forward declaring class Animation to prevent circular dependency.
	// therefore i need to declare animation as a pointer because its just a header
	// and we know there will be a class as animation included in the cpp and will be there in the declaration
	
	Animation*              mCurrentAnimation;
    int                     mRefCount;

							Entity(SDL_Texture* tex);
							Entity(SDL_Texture* tex, int numCells, float duration, bool loopable);							
							Entity(SDL_Texture* tex, int numCells, float duration, bool loopable, int hitboxWidth, int hitboxHeight);
							//Entity::Entity(SDL_Texture* tex, int numCells, float duration, const Vec2& pos)

    virtual                 ~Entity();

                            // need to call this unless sprite faces right in texture
    virtual void            SetSpriteAngleCorrection(CardinalDir spriteForwardDir);

    virtual const Vec2&     Center() const      { return mCenterPos; }

    virtual void            SetCenter(const Vec2& pos)  { mCenterPos = pos; }
    virtual void            SetCenter(float x, float y) { mCenterPos = Vec2(x, y); }

	virtual float			Width() const { return mTexWidth; }
	virtual float			Height() const { return mTexHeight; }

    virtual float           Health() const      { return mHealth; }
    virtual void            SetHealth(float h)  { mHealth = h; }

    virtual bool            IsDead() const      { return mHealth <= 0; }
    virtual bool            IsAlive() const     { return mHealth > 0; }

	virtual float           Radius() const { return mRadius; }
	// x is half the texture width - half the hitbox width
	//
	// mCenterPos.x = 
	//
	//rect(hitboxX + (rectWidth / 2) / 2, hitboxY + (rectWidth / 2) / 2, rectWidth / 2, rectHeight / 2);
	virtual float			HitBoxLeft() const  { return mCenterPos.x - (mHitbox->w / 2); }
	virtual float			HitBoxRight() const { return mCenterPos.x + (mHitbox->w / 2); }
	virtual float			HitBoxTop() const   { return mCenterPos.y - (mHitbox->h / 2); }
	virtual float			HitBoxBottom() const{ return mCenterPos.y + (mHitbox->h / 2); }

	virtual float			Left() const		{ return mCenterPos.x - (mTexWidth / 2); }
	virtual float			Top() const			{ return mCenterPos.y - (mTexHeight / 2); }
	virtual float			Bottom() const		{ return mCenterPos.y + (mTexHeight / 2); }
	virtual float			Right() const		{ return mCenterPos.x + (mTexWidth / 2); }

	virtual void			SetState(mState newState) { };//void Player::SetState(mState newState)
	virtual void            SetLeft(float x) { mCenterPos.x = x + (mTexWidth / 2); }
	virtual void            SetRight(float x) { mCenterPos.x = x - (mTexWidth / 2); }
	virtual void            SetTop(float y) { mCenterPos.y = y + (mTexHeight / 2); }
	virtual void            SetBottom(float y) { mCenterPos.y = y - (mTexHeight / 2); }

	// virtual void         SetLeft(float x)    { mCenterPos.x = x + (mTexWidth / 2); }
    //virtual void          SetRight(float x)   { mCenterPos.x = x - (mTexWidth / 2); }
    //virtual void          SetTop(float y)     { mCenterPos.y = y + (mTexHeight / 2); }
    //virtual void          SetBottom(float y)  { mCenterPos.y = y - (mTexHeight / 2); }

    virtual float           Angle() const       { return mAngle; }
    virtual void            SetAngle(float deg) { mAngle = deg; }

	virtual unsigned short int      Layer() const { return mLayer; }
	virtual void			SetLayer(unsigned short int layer) { mLayer = layer; }

	virtual void            SetLayer(float deg) { mAngle = deg; }

    virtual void            SetOrientation(CardinalDir dir) { this->SetAngle(CardinalAngle[dir]); }

    virtual Vec2            ForwardDir() const;
    virtual void            SetForwardDir(const Vec2& dir);
    virtual void            SetForwardDir(float dx, float dy)       { SetForwardDir(Vec2(dx, dy)); }

	virtual void		    Update(float dt)    = 0;

	virtual void			Draw(SDL_Renderer* renderer, Camera* camera, SDL_RendererFlip flipType = SDL_FLIP_NONE) const;

	virtual void			setAnimation(SDL_Texture* tex, int numCells, float duration, bool loopable, SDL_RendererFlip flip = SDL_FLIP_NONE);
	virtual void			setAnimationTexture(SDL_Texture* tex);

	virtual void	        addTimeToAnimation(float dt);
	virtual void			setState(mState x) {};

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
