#ifndef COLORCHANGE_H_
#define COLORCHANGE_H_

#include <SDL.h>

#include "Math.h"
//class Camera;

const class ColorChange
{
	SDL_Texture*                mTex;
	SDL_RendererFlip			mFlipType;

	int                         mColorRange;

	float                       mDuration;          // in seconds
	bool                        mIsLoopable;

public:
	ColorChange(SDL_Texture* tex, int colorRange);
	~ColorChange();
	 
	int							mCurrentRedValue;              // current time position (in seconds)
	int							mCurrentGreenValue;
	int							mCurrentBlueValue;

	//we want to draw only sometimes
	void						Draw(SDL_Texture* tex);
	void						IncrementRedValue(int incrementValue);

	//do i increment everytime or do i limit the number? do i loop? why not code all! then see what i use... :)
	void						IncrementRedValue_WithLoop(int incrementValue, int WhatNumberToLoopAt);
	bool						IncrementRedValue_WithLimit(int incrementValue, int maxRange);
	void						DecrementRedValue_WithLimit(int DecrementValue, int StoppingValue);

	//*&^%$$#@! *&^%$$#@! WE NEED TO ADD A MULTIPLY FUNCTION AS WELL FOR COOL EFFECTS. DO IT TO ALL VALUES OR SOME IN DIFFERENT ORDERS. LIGHTSHOWS  *&^%$$#@! *&^%$$#@!

	void						SetTexture(SDL_Texture* tex);
	void						ResetColors();

};

#endif
