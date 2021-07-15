#include "Game.h"

#include <iostream>

void Game::ProcessEvents()
{
    
	while (SDL_PollEvent(&mE)) {
		switch (mE.type) {
		case SDL_QUIT:
			OnQuit();
			break;
		case SDL_KEYDOWN:
			OnKeyDown(mE.key);
			break;
		case SDL_KEYUP:
			OnKeyUp(mE.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
			OnMouseDown(mE.button);
			break;
		case SDL_MOUSEBUTTONUP:
			OnMouseUp(mE.button);
			break;
		case SDL_MOUSEMOTION:
			OnMouseMotion(mE.motion);
			break;
		case SDL_WINDOWEVENT:
			if (mE.window.event == SDL_WINDOWEVENT_RESIZED) {
				OnWindowResized(mE.window.data1, mE.window.data2);
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
