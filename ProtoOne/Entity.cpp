#include "Entity.h"
#include "Camera.h"
#include "Animation.h"
#include "LayerManager.h"

Entity::Entity(SDL_Texture* tex)
    : mTex(tex)
    , mTexWidth(0)
    , mTexHeight(0)
    , mAngle(0)
    , mHealth(100)
    , mRefCount(0)
	, mLayer(1)
	, mActionable(false)
	
{
	mColorChange = new ColorChange(tex, 255);
	SDL_QueryTexture(tex, NULL, NULL, &mTexWidth, &mTexHeight);

	mHitbox = new SDL_Rect();
	mHitbox->w = mTexWidth;
	mHitbox->h = mTexHeight;

    // default radius is average of half width and half height
    //mRadius = 0.5f * (0.5f * mTexWidth + 0.5f * mTexHeight);
    mRadius = 0.25f * (mTexWidth + mTexHeight);
	mInCutscene = false;
};

//Entity With Animation
Entity::Entity(SDL_Texture* tex, int numCells, float duration, bool loopable)
	: mTex(tex)
	, mTexWidth(0)
	, mTexHeight(0)
	, mAngle(0)
	, mHealth(100)
	, mRefCount(0)
	, mLayer(1)
	, mActionable(false)
{
	mColorChange = new ColorChange(tex, 0);
	SDL_QueryTexture(tex, NULL, NULL, &mTexWidth, &mTexHeight);
	//so, i have to do something similar
	  
	//mRadius = 0.5f * (0.5f * mTexWidth + 0.5f * mTexHeight);
	mRadius = 0.25f * ((mTexWidth / numCells) + mTexHeight);

	mHitbox = new SDL_Rect();
	mHitbox->w = (mTexWidth / numCells);
	mHitbox->h = mTexHeight;

	//rect(hitboxX + (rectWidth / 2) / 2, hitboxY + (rectWidth / 2) / 2, rectWidth / 2, rectHeight / 2);
	//by default the hitbox will be
	 
	//dedouple this later so we can have any type of animation later.
	mCurrentAnimation = new Animation(tex, numCells, duration, loopable);
	//setAnimation();
	mInCutscene = false;
};

//Constructor for an Entity With an Animation and HITBOX
Entity::Entity(SDL_Texture* tex, int numCells, float duration, bool loopable, int hitboxWidth, int hitboxHeight)
	: mTex(tex)
	, mTexWidth(0)
	, mTexHeight(0)
	, mAngle(0)
	, mHealth(100)
	, mRefCount(0)
	, mLayer(1)
	, mActionable(false)
{
	mColorChange = new ColorChange(tex, 0);
	SDL_QueryTexture(tex, NULL, NULL, &mTexWidth, &mTexHeight);
	//so, i have to do something similar

	// default radius is average of half width and half height
	//mRadius = 0.5f * (0.5f * mTexWidth + 0.5f * mTexHeight);

	//no radius because theyre rectangles not circles!!!
	//mRadius = 0.25f * ((mTexWidth / numCells) + mTexHeight);
	
	//UPDATE ME TO JUST USE THE TEXT WIDTH AND HEIGHT INSTEAD OF RADIUS. need to update Left and top values. THIS IS WHY IM SEEING VISUAL GLITCHES AND CLIISION ISSUES
	mRadius = 0.25f * ((mTexWidth / numCells) + mTexHeight);

	//SET THE HITBOX AFTER YOU KNOW THE CENTER POS. OR JUST MAKE THE HITBOX VALUES RELATIVE TO THE CENTERPOS LIKE LEFT() AND RIGHT() functions are done
	//(going with both for more customizability and it doenst take long (i.e. good value))
	mHitbox = new SDL_Rect();
	mHitbox->w = hitboxWidth;
	mHitbox->h = hitboxHeight;
		
	//dedouple this later so we can have any type of animation later.
	mCurrentAnimation = new Animation(tex, numCells, duration, loopable);
	
	//setAnimation();
	mInCutscene = false;
};

Entity::~Entity()
{
	if (mCurrentAnimation != nullptr) {
		mCurrentAnimation;
	}
}

void Entity::Draw(SDL_Renderer* renderer, Camera* camera,  SDL_RendererFlip flipType) const
{
    // check if we have a valid texture
    if (mTex) {

        // compute on-screen rectangle for this entity
        SDL_Rect screenRect;
        screenRect.w = mTexWidth;
        screenRect.h = mTexHeight;
        screenRect.x = camera->WorldToScreenX(mCenterPos.x - 0.5f * mTexWidth);
        screenRect.y = camera->WorldToScreenY(mCenterPos.y - 0.5f * mTexHeight);

        // compute point about which to rotate the sprite texture
        SDL_Point rotCenter = { mTexWidth / 2, mTexHeight / 2 };

        // account for sprite orientation in the texture
        float rotAngle = mAngle - mSpriteAngleOffset;

		if (mCurrentAnimation != NULL) {
			mCurrentAnimation->Draw(renderer, mCenterPos, camera);
		} else {
			//draw stationary texture if no current animation. replace this with, say an idle animation maybe?
			SDL_RenderCopyEx(renderer, mTex, NULL, &screenRect, rotAngle, &rotCenter, flipType);
		}
    }
}

void Entity::setAnimation(SDL_Texture* tex, int numCells, float duration, bool loopable, SDL_RendererFlip flipType)
{ 
	//TODO:
	//will need to change this when i work with different sprites acting upon each other. 
	//we want the width of the sprite to be smaller than the image!!!!!!!!!	
	 
	SDL_QueryTexture(tex, NULL, NULL, &mTexWidth, &mTexHeight);
	mCurrentAnimation->mFlipType = flipType;
	mCurrentAnimation->mCellWidth = mTexWidth / numCells;
	mCurrentAnimation->mCellHeight = mTexHeight;
	mCurrentAnimation->SetCurrentTexture(tex);
	mCurrentAnimation->mNumCells = numCells;
	mCurrentAnimation->mDuration = duration;	
	mCurrentAnimation->mIsLoopable = loopable;
	mColorChange->SetTexture(tex);
	

}

void Entity::setAnimationTexture(SDL_Texture* tex) {
	mCurrentAnimation->SetCurrentTexture(tex);	
}

void Entity::addTimeToAnimation(float dt)
{
	mCurrentAnimation->AddTime(dt);
}


