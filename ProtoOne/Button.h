#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL.h>

class Button
{
	SDL_Rect				mRect;
    SDL_Color				mColor;     // fallback color, in case there are problems with the texture

    SDL_Texture*            mTex;                                                   // <--- ###

public:
							Button();
							Button(SDL_Texture* tex);
                            Button(int x, int y, SDL_Texture* tex);
							Button(int x, int y, SDL_Texture* tex, const SDL_Color& color);

                            ~Button();

	int						Width() const		{ return mRect.w; }
	int						Height() const		{ return mRect.h; }

	int						Left() const		{ return mRect.x; }
	int						Top() const			{ return mRect.y; }
	int						Right() const		{ return mRect.x + mRect.w; }
	int						Bottom() const		{ return mRect.y + mRect.h; }

	SDL_Point				Center() const;

	void					SetLeft(int x)		{ mRect.x = x; }
	void					SetRight(int x)		{ mRect.x = x - mRect.w; }
	void					SetTop(int y)		{ mRect.y = y; }
	void					SetBottom(int y)	{ mRect.y = y - mRect.h; }

	void					SetWidth(int w)		{ mRect.w = w; }
	void					SetHeight(int h)	{ mRect.h = h; }

	void					SetCenter(int x, int y);

	void					SetColor(SDL_Color color)						{ mColor = color; }
	void					SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)	{ mColor.r = r; mColor.g = g; mColor.b = b; mColor.a = a; }

	void					Draw(SDL_Renderer* renderer) const;

    bool                    Contains(int x, int y) const;

private:
	static const int		msDefaultWidth;
	static const int		msDefaultHeight;

	static const SDL_Color	msDefaultColor;
};

#endif
