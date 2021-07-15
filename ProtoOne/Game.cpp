#include "Game.h"
#include "Gameplay.h"
#include "MainMenu.h"
#include "Sound.h"
#include <SDL_image.h>
#include <iostream>
Game::Game()
//1024
	: mScreenWidth(1920)
	, mScreenHeight(1080)
	, mWindow(NULL)
	, mKeys(NULL)
	, mRenderer(NULL)
	, mShouldQuit(false)
    , mGameplayState(NULL)
    , mMainMenuState(NULL)
	, mCurrentState(NULL)
	, mSound(NULL)
{
}

bool Game::Run()
{
	// setup
	if (!Initialize()) {
		std::cerr << "*** Game initialization failed" << std::endl;
		return false;
	}

    Uint32 prevTicks = SDL_GetTicks();

	// game loop
	while (!mShouldQuit) {

        Uint32 ticks = SDL_GetTicks();

        float dt = 0.001f * (ticks - prevTicks);    // delta time (in seconds)        

		if(mScriptProcessor.userControlEnabled) {
			ProcessEvents();
		}

		Update(dt);

		Draw(dt);

		mScriptProcessor.ProcessActions(dt);
		
		// tell the renderer to display everything we just drew
		SDL_RenderPresent(mRenderer);

		 prevTicks = ticks;
	}

	// cleanup
	Shutdown();

	return true;
}

bool Game::Initialize()
{
    std::cout << "Initializing game" << std::endl;
	
	//
	// initialize SDL
	//
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "*** Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return false;
	}

    // initialize SDL_image add-on
    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {                                           // <--- ###
        std::cerr << "*** Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return 1;
    }

	//
	// create a window
	//
	mWindow = SDL_CreateWindow("Hello, SDL2!",
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								mScreenWidth, mScreenHeight,
								SDL_WINDOW_SHOWN);
	if (!mWindow) {
		std::cerr << "*** Failed to create window: " << SDL_GetError() << std::endl;
		return false;
	}

	//
	// get a pointer to keyboard state managed by SDL
	//
	mKeys = SDL_GetKeyboardState(NULL);

	//
	// get a pointer to keyboard state managed by SDL
	//
	mKeys = SDL_GetKeyboardState(NULL);

	//SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	//SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

	/*
	*  \sa SDL_RenderGetLogicalSize()
	*  \sa SDL_RenderSetScale()
	*  \sa SDL_RenderSetViewport()
	*/
	//Rendering resolution
	SDL_RenderSetLogicalSize(mRenderer, 1920, 1080);
	//SDL_RenderSetLogicalSize(mRenderer, 1280, 720);

	//
	// create a renderer that takes care of drawing stuff to the window
	//
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		std::cerr << "*** Failed to create renderer: " << SDL_GetError() << std::endl;
		return false;
	}

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); 
	//
	//initialize the sounds
	//
	mSound = new Sound();
	mSound->Init();
	//
    // create all game states
    //
    mGameplayState = new Gameplay(this, mSound);
    mGameplayState->Initialize();

    mMainMenuState = new MainMenu(this, mSound);
    mMainMenuState->Initialize();

	//we cant initialize EVERYTHING now...not enough ram. will need to initiialize future levels at the
	//time they are used. for example, battles, and different areas and groups of areas

    // set initial state
    mCurrentState = mMainMenuState;

	return true;
}

void Game::Shutdown()
{
    std::cout << "Shutting down game" << std::endl;

    mGameplayState->Shutdown();
    mMainMenuState->Shutdown();

    delete mGameplayState;
    delete mMainMenuState;

    // unload the image loading library
    IMG_Quit();                                                                     // <--- ###

	// this closes the window and shuts down SDL
    SDL_Quit();
}

void Game::Update(float dt)
{
    mCurrentState->Update(dt);
}

void Game::Draw(float dt)
{
    mCurrentState->Draw(dt);

}

void Game::EnterMainMenu()
{
    mCurrentState = mMainMenuState;
}

void Game::EnterGameplay()
{
    mCurrentState = mGameplayState;
}

float Game::GetTime() const
{
    return 0.001f * SDL_GetTicks();
}
