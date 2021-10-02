
#ifndef BOUNDARY_H_
#define BOUNDARY_H_

#include <SDL.h> 
#include "Camera.h"

class Boundary
{
public:

	SDL_Rect*						mBoundaryRect;
	Vec2 mCenter;


	Boundary(int x, int  y, int  w, int  h)
	{

		mBoundaryRect = new SDL_Rect();
		mBoundaryRect->x = x;
		mBoundaryRect->y = y;
		mBoundaryRect->w = w;
		mBoundaryRect->h = h;
		mCenter.x = w * 0.5f;
		mCenter.y = h * 0.5f;

	};

	~Boundary() {
		if (mBoundaryRect != nullptr) {
			delete mBoundaryRect;
		}
	}

	void Draw(SDL_Renderer* r, Camera* cam) {	
		
		SDL_Rect rect;
		rect.x = cam->WorldToScreenX(mBoundaryRect->x);
		rect.y = cam->WorldToScreenY(mBoundaryRect->y);
		rect.w = mBoundaryRect->w;
		rect.h = mBoundaryRect->h;
	 
		SDL_RenderFillRect(r, &rect);
	}
};

#endif
