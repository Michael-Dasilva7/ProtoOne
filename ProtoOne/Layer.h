
#ifndef LAYER_H_
#define LAYER_H_

#include <SDL.h>
#include "Texture.h"


class Layer
{
	SDL_Texture*					mTex;
	SDL_RendererFlip				mFlipType;
	SDL_Rect						mSrcRect;
	SDL_Rect						mDestRect;
 
};

#endif
