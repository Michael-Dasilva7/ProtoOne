#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
 
#undef main
#include "Gameplay.h"
#include "MainMenu.h"
#include "Actions.h"
#include "LayerManager.h"
#include "Sound.h"
class Game
{
	int					mScreenWidth;
	int					mScreenHeight;

	SDL_Window*			mWindow;
	const Uint8*		mKeys;
	SDL_Renderer*		mRenderer;

	bool				mShouldQuit;

    Gameplay*           mGameplayState;
    MainMenu*           mMainMenuState;
    GameState*          mCurrentState;

	LayerManager*       mLayerManager;
	Sound*				mSound;

public:
						Game();

	bool				Run();

    int                 GetScreenWidth() const      { return mScreenWidth; }
    int                 GetScreenHeight() const     { return mScreenHeight; }

    SDL_Renderer*       GetRenderer() const         { return mRenderer; }
    const Uint8*        GetKeys() const             { return mKeys; }

    Gameplay*           GetGameplayState() const    { return mGameplayState; }
    MainMenu*           GetMainMenuState() const    { return mMainMenuState; }
	//ResourceManager*    GetResourceManager() const  { return mResourceManager; }

	SDL_Event			mE;

    void                EnterMainMenu();
    void                EnterGameplay();

    float               GetTime() const;

    void                Quit()                      { mShouldQuit = true; }

	aScriptProcessor    mScriptProcessor;

	bool				mDebugMode;
private:
	bool				Initialize();
	void				Shutdown();

	void				ProcessEvents();

	void				OnQuit();

    void				OnWindowResized(int w, int h);
	void				OnKeyDown(const SDL_KeyboardEvent& kbe);
	void				OnKeyUp(const SDL_KeyboardEvent& kbe);
	void				OnMouseDown(const SDL_MouseButtonEvent& mbe);
	void				OnMouseUp(const SDL_MouseButtonEvent& mbe);
	void				OnMouseMotion(const SDL_MouseMotionEvent& mme);

	void				Update(float dt);
	void				Draw(float dt);

};


Game* GetGame();


#endif
