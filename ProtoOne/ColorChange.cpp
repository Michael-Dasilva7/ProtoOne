#include "ColorChange.h"

#include <cmath>

#include <iostream>

#include <string>

ColorChange::ColorChange(SDL_Texture* tex, int colorRange)//add loopable later
	: mTex(tex)
	, mColorRange(colorRange)
	//, mIsLoopable(loopable)//add loopable later
	, mCurrentRedValue(0)
	, mCurrentGreenValue(55)
	, mCurrentBlueValue(55)
{
}

ColorChange::~ColorChange()
{

	// nothing to destroy
}


void ColorChange::Draw(SDL_Texture* tex)
{
	if (mTex != NULL) {
		//std::cout << "Inside Colorchange Draw method. Current values are as follows: " + std::to_string(mCurrentRedValue) << std::endl;
		SDL_SetTextureColorMod(mTex, mCurrentRedValue, mCurrentGreenValue, mCurrentBlueValue);
 	}
}


void ColorChange::IncrementRedValue(int incrementValue) {
	mCurrentRedValue += incrementValue;
}

void ColorChange::IncrementRedValue_WithLoop(int incrementValue, int WhatNumberToLoopAt)
{
	int nextRedValue = mCurrentRedValue += incrementValue;
	if (nextRedValue >= WhatNumberToLoopAt){
		mCurrentRedValue = 0;
	}
	else
	{
		mCurrentRedValue = nextRedValue;
	}
}

//make a new method to loop the amount of times
bool ColorChange::IncrementRedValue_WithLimit(int incrementValue, int maxRange)
{
	//check if were at or over range, and if so, set to max value and return true;
	bool isAtMaxRangevalue = false;
	int nextRedValue = mCurrentRedValue += incrementValue;
	if (nextRedValue >= maxRange) {	
		isAtMaxRangevalue = true;
	}else{
		mCurrentRedValue = nextRedValue;
	}
	return isAtMaxRangevalue;
}

void ColorChange::DecrementRedValue_WithLimit(int DecrementValue, int StoppingValue)
{
	int nextRedValue = mCurrentRedValue -= DecrementValue;
	if (nextRedValue <= StoppingValue) {

	}
	else
	{
		mCurrentRedValue = nextRedValue;
	}
}

void ColorChange::SetTexture(SDL_Texture* tex) {
	mTex = tex;
}

void ColorChange::ResetColors()
{
	mCurrentRedValue = 0;
	mCurrentGreenValue = 0;
	mCurrentBlueValue = 0;
}