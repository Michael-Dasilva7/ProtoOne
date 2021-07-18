#include "Animation.h" 
#include <cmath>

Animation::Animation(SDL_Texture* tex, int numCells, float duration, bool loopable, SDL_RendererFlip flipType, bool isBackground)
    : mTex(tex)
    , mNumCells(numCells)
    , mDuration(duration)
    , mIsLoopable(loopable)
    , mCellWidth(0)
    , mCellHeight(0)
    , mTime(0.0f)

{
    int texWidth = 0, texHeight = 0;
    SDL_QueryTexture(tex, NULL, NULL, &texWidth, &texHeight);
	//TODO:
	//will need to change this when i work with different sprites acting upon each other. we want the width of the sprite to be smaller than the image!!!!!!!!!

    mCellWidth = texWidth / numCells;
    mCellHeight = texHeight;
	mFlipType = flipType;
	mIsBackground = isBackground;
}

Animation::~Animation()
{
    // nothing to destroy
}

void Animation::AddTime(float dt)
{
    mTime += dt;

    // see if we've reached or passed the end
    if (mTime >= mDuration) {
        if (mIsLoopable) {
            // wrap around
            mTime = std::fmod(mTime, mDuration);
        } else {
            // cap
            mTime = mDuration;
        }
    }
}

void Animation::Draw(SDL_Renderer* renderer, const Vec2& pos, Camera* camera) const
{
    int cellIndex;
	//there shouldnt be negative time, but if there is, we only wanna display the first cell
    if (mTime <= 0.0f) {
        cellIndex = 0;
    } else if (mTime >= mDuration) {
		//If the amount of time passed since last draw call is greater than or equal to the duration of our animation,
		// then display the last cell of the sprite/texture
        cellIndex = mNumCells - 1;
    } else {
		//if the delta time(time passed since last frame) is greater than 0 and less than duration of the animation 
		//we want to perform the following trick. this will find the right frame of animation
		// based on how long the animation takes to loop and how much time has passed. 
		//  for example, if there are 4 cells in an animation, then we want each cell to have equal time
		// displaying. so since we track time passed with mTime variable, and we have the duration, we know which frame to be on.
		/*
		i.e., 
		number of cells = 4
		mTime = 1
		mDuration = 4

		mtime / mDuration = x
		1 / 4 = x
		0.25 = x
		
		x * number of cells = y

		0.25 * 4 = 1
		1st cell!

		now, another second passes and now we're at mtime = 2

		2/4 = 0.5
		0.5 * 4(num of cells) = 2
		2nd cell!

		now what if only half a second passes?!? what frame?? ....

		2.5 / 4 = 0.625
		0.625 * 4 = 2.5
		2.5 casted to an int is 2(it's truncated)

		still second frame! the math works! :) (trust me its tested). man, who thought of this simple effective trick?!
		only problem is,....

		*/
        cellIndex = (int)(mTime / mDuration * mNumCells);
    }

    SDL_Rect texRect;
    texRect.w = mCellWidth;
    texRect.h = mCellHeight;
    texRect.x = cellIndex * mCellWidth;
    texRect.y = 0;

    // compute on-screen rectangle for this entity
    SDL_Rect screenRect;
    screenRect.w = mCellWidth;
    screenRect.h = mCellHeight;


	if (mIsBackground) {
		screenRect.x =pos.x;
		screenRect.y = pos.y;
	}
	else
	{
		screenRect.x = camera->WorldToScreenX(pos.x - 0.5f * mCellWidth);
		screenRect.y = camera->WorldToScreenY(pos.y - 0.5f * mCellHeight);
	}

    SDL_RenderCopyEx(renderer, mTex, &texRect, &screenRect, 0.0, NULL, mFlipType);


	//	player center x : 348
	//	player center y : -157
	//	World To ScreenX(using 0 as position x and 4096) : -2395
	//	World To ScreenY(using 0 as position x and 3469) : -1577
	//	camera view left : 348
	//	camera view camera top : -157
	//	camera view bottom : 867
	//	camera view right : 2024
	//	screen width 1676
	//	screen height : 1024

}

void Animation::Draw(SDL_Renderer* renderer, const Vec2& pos, Camera* camera, Uint8 alpha) const
{
	int cellIndex;
	//there shouldnt be negative time, but if there is, we only wanna display the first cell
	if (mTime <= 0.0f) {
		cellIndex = 0;
	}
	else if (mTime >= mDuration) {
		//If the amount of time passed since last draw call is greater than or equal to the duration of our animation,
		// then display the last cell of the sprite/texture
		cellIndex = mNumCells - 1;
	}
	else {
		cellIndex = (int)(mTime / mDuration * mNumCells);
	}

	SDL_Rect texRect;
	texRect.w = mCellWidth;
	texRect.h = mCellHeight;
	texRect.x = cellIndex * mCellWidth;
	texRect.y = 0;

	// compute on-screen rectangle for this entity
	SDL_Rect screenRect;
	screenRect.w = mCellWidth;
	screenRect.h = mCellHeight;

	if (mIsBackground) {
		screenRect.x = pos.x;
		screenRect.y = pos.y;
	}
	else
	{
		screenRect.x = camera->WorldToScreenX(pos.x - 0.5f * mCellWidth);
		screenRect.y = camera->WorldToScreenY(pos.y - 0.5f * mCellHeight);
		
	}
	SDL_SetTextureAlphaMod(mTex, alpha);
	
	//Add Layer 
	SDL_RenderCopyEx(renderer, mTex, &texRect, &screenRect, 0.0, NULL, mFlipType);
	/*
	SDL_RenderDrawLine
	SDL_RenderDrawPoint
	SDL_RenderDrawRect
	SDL_SetRenderDrawBlendMode
	SDL_SetRenderDrawColor
	*/
}

//increases speed and float value is expected in seconds
////set duration to test speeding or slowing and returns current speed in seconds
float Animation::increaseAnimationSpeed(float addedDuration) {
	mDuration += addedDuration;
	return mDuration;
}

SDL_Texture* Animation::GetCurrentTexture() {
	return mTex;
}

void Animation::SetCurrentTexture(SDL_Texture* tex) {
	mTex = tex;
}

void Animation::Reset() {
	mTime = 0.0f;
}