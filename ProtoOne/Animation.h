#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <SDL.h>

#include "Math.h"
#include "Camera.h"
//class Camera;

class Animation
{
    SDL_Texture*					mTex;
	
    float							mTime;              // current time position (in seconds)	

public:	
									Animation(SDL_Texture* tex, int numCells, float duration, bool loopable, SDL_RendererFlip flipType = SDL_FLIP_NONE, bool isBackground = false);
									
									~Animation();
	
	SDL_RendererFlip				mFlipType;
	int								mCellWidth;
	int								mCellHeight;
	int								mNumCells;
	float							mDuration;          // in seconds
	bool							mIsLoopable;

	bool							mIsBackground;		//used to change how camera acts. we want it to be stationary if a background and not change based on camera

	void							AddTime(float dt);

    bool							IsDone() const      { return mTime >= mDuration; }

    void							Draw(SDL_Renderer* renderer, const Vec2& pos, Camera* camera) const;
	void							Draw(SDL_Renderer* renderer, const Vec2& pos, Camera* camera, Uint8 alpha) const;

	float							increaseAnimationSpeed(float addedDuration);

	SDL_Texture* 					GetCurrentTexture();

	void							SetCurrentTexture(SDL_Texture* tex);
	void							Reset();
};

#endif
