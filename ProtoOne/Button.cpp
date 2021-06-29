#include <iostream>

#include "Button.h"

const int Button::msDefaultWidth = 64;
const int Button::msDefaultHeight = 64;

const SDL_Color	Button::msDefaultColor = { 255, 255, 255, 255 };		// white


Button::Button()
	: Button(0, 0, NULL, msDefaultColor)		// C++11 delegating constructors ftw
{
}

Button::Button(SDL_Texture* tex)
	: Button(0, 0, tex, msDefaultColor)
{
}

Button::Button(int x, int y, SDL_Texture* tex)
	: Button(x, y, tex, msDefaultColor)
{
}

Button::Button(int x, int y, SDL_Texture* tex, const SDL_Color& color)
	: mRect()
	, mColor()
    , mTex(tex)
{
    int w = msDefaultWidth;
    int h = msDefaultHeight;

    // if the texture is not NULL, get its size
    if (tex) {
        if (SDL_QueryTexture(tex, NULL, NULL, &w, &h) < 0) {                                        // <--- ###
            std::cerr << "*** Failed to query player texture: " << SDL_GetError() << std::endl;
        } 
    }

	mRect.w = w;
	mRect.h = h;
	
    mRect.x = x;
	mRect.y = y;

    mColor = color;		// struct bitwise copy
}

Button::~Button()
{
    std::cout << "Button destroyed" << std::endl;
}

SDL_Point Button::Center() const
{
	SDL_Point p;
	p.x = Left() + Width() / 2;
	p.y = Top() + Height() / 2;
	return p;
}

void Button::SetCenter(int x, int y)
{
	SetLeft(x - Width() / 2);
	SetTop(y - Height() / 2);
}

void Button::Draw(SDL_Renderer* renderer) const
{
    // check if we have a valid texture
    if (mTex) {
        // draw textured rectangle
        SDL_RenderCopy(renderer, mTex, NULL, &mRect);                               // <--- ###
    } else {
        // draw solid rectangle
        SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderFillRect(renderer, &mRect);
    }
}

bool Button::Contains(int x, int y) const
{
    return x >= Left() && x < Right() && y >= Top() && y < Bottom();
}
