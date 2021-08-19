#include "Entity.h"
#include "Camera.h"
#include "Animation.h"
#include "LayerManager.h"

Entity::Entity(SDL_Texture* tex)
    : mTex(tex)
    , mTexWidth(0)
    , mTexHeight(0)
    , mAngle(0)
    , mRadius(0)
    , mHealth(100)
    , mRefCount(0)
	, mLayer(1)
	
{
	mColorChange = new ColorChange(tex, 255);
	SDL_QueryTexture(tex, NULL, NULL, &mTexWidth, &mTexHeight);
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
	, mRadius(0)
	, mHealth(100)
	, mRefCount(0)
	, mLayer(1)
{
	mColorChange = new ColorChange(tex, 0);
	SDL_QueryTexture(tex, NULL, NULL, &mTexWidth, &mTexHeight);

	// default radius is average of half width and half height
	//mRadius = 0.5f * (0.5f * mTexWidth + 0.5f * mTexHeight);
	mRadius = 0.25f * ((mTexWidth / numCells) + mTexHeight);
	//dedouple this later so we can have any type of animation later.
	mCurrentAnimation = new Animation(tex, numCells, duration, loopable);
	//setAnimation();
	mInCutscene = false;
};

Entity::~Entity()
{
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