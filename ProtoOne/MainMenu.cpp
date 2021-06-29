#include "MainMenu.h"
#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

#include <iostream>

MainMenu::MainMenu(Game* game)
    : GameState(game)
    , mBtnNewTex(NULL)
    , mBtnResumeTex(NULL)
    , mBtnExitTex(NULL)
    , mBtnNew(NULL)
    , mBtnResume(NULL)
    , mBtnExit(NULL)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Initialize()
{
    SDL_Renderer* renderer = mGame->GetRenderer();
    mBtnNewTex = LoadTexture("media/button_new.png", renderer);
    mBtnResumeTex = LoadTexture("media/button_resume.png", renderer);
    mBtnExitTex = LoadTexture("media/button_exit.png", renderer);
	mBackgroundTex = LoadTexture("media/volcanoVSundine.png", renderer);

    mBtnNew = new Button(mBtnNewTex);
    mBtnResume = new Button(mBtnResumeTex);
    mBtnExit = new Button(mBtnExitTex);

    int cx = mGame->GetScreenWidth() / 2;
    int cy = mGame->GetScreenHeight() / 2;

    mBtnResume->SetCenter(cx, cy - 100);
    mBtnNew->SetCenter(cx, cy);
    mBtnExit->SetCenter(cx, cy + 100);
}

void MainMenu::Shutdown()
{
    delete mBtnNew;
    delete mBtnResume;
    delete mBtnExit;

    SDL_DestroyTexture(mBtnNewTex);
    SDL_DestroyTexture(mBtnResumeTex);
    SDL_DestroyTexture(mBtnExitTex);
}

void MainMenu::Update(float dt)
{
}

void MainMenu::Draw(float dt)
{
    SDL_Renderer* renderer = mGame->GetRenderer();

    if (mGame->GetGameplayState()->IsActive()) {

        mGame->GetGameplayState()->Draw(dt);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
       // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 192);
		//volcanoVSundine.png
		SDL_RenderFillRect(renderer, NULL);

        mBtnResume->Draw(renderer);

    } else {
		//mBackgroundTex 
		int texWidth = 0, texHeight = 0;
		//SDL_QueryTexture(mBackgroundTex, NULL, NULL, &texWidth, &texHeight);
		 
		SDL_Rect bgRect;
		bgRect.x = mGame->GetScreenWidth() / 4;
		bgRect.y = 0;
		bgRect.w = texWidth;
		bgRect.h = texHeight;

		SDL_RenderCopy(renderer, mBackgroundTex, NULL, &bgRect);

		//DRAW A BLACK BACKGROUND:
		//renderer / R / G / B / A;
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);
    }

    mBtnNew->Draw(renderer);
    mBtnExit->Draw(renderer);
}

void MainMenu::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
    switch (kbe.keysym.sym) {
    case SDLK_ESCAPE:
        std::cout << "User pressed Escape" << std::endl;
        //mGame->mShouldQuit = true;  // set quit flag
        mGame->EnterGameplay();
        break;
    }
}

void MainMenu::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
    if (mbe.button == SDL_BUTTON_LEFT) {

        if (mGame->GetGameplayState()->IsActive()) {
            if (mBtnResume->Contains(mbe.x, mbe.y)) {
                mGame->EnterGameplay();
            }
        }

        if (mBtnNew->Contains(mbe.x, mbe.y)) {
            mGame->GetGameplayState()->LoadLevel();     // reset everything
            mGame->EnterGameplay();
        }

        if (mBtnExit->Contains(mbe.x, mbe.y)) {
            mGame->Quit();
        }
    }
}
