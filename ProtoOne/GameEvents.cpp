#include "Game.h"

#include <iostream>

void Game::ProcessEvents()
{
    SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			OnQuit();
			break;
		case SDL_KEYDOWN:
			OnKeyDown(e.key);
			break;
		case SDL_KEYUP:
			OnKeyUp(e.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
			OnMouseDown(e.button);
			break;
		case SDL_MOUSEBUTTONUP:
			OnMouseUp(e.button);
			break;
		case SDL_MOUSEMOTION:
			OnMouseMotion(e.motion);
			break;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				OnWindowResized(e.window.data1, e.window.data2);
			}
			break;
		default:
			break;
		}
	}
}

void Game::OnQuit()
{
	std::cout << "Application was told to quit" << std::endl;

	mShouldQuit = true;
}

void Game::OnWindowResized(int w, int h)
{
	std::cout << "Window resized to " << w << 'x' << h << std::endl;

	mScreenWidth = w;
	mScreenHeight = h;
	// We need to update the screen correctly when its resized
	// 
    mCurrentState->OnWindowResized(w, h);
}

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
	mCurrentState->OnKeyDown(kbe);
}

void Game::OnKeyUp(const SDL_KeyboardEvent& kbe)
{
    mCurrentState->OnKeyUp(kbe);
}

void Game::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
	mCurrentState->OnMouseDown(mbe);
}

void Game::OnMouseUp(const SDL_MouseButtonEvent& mbe)
{
    mCurrentState->OnMouseUp(mbe);
}

void Game::OnMouseMotion(const SDL_MouseMotionEvent& mme)
{
    mCurrentState->OnMouseMotion(mme);
}
