#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#include "GameState.h"
#include "Button.h"

class MainMenu : public GameState
{

    SDL_Texture*        mBtnNewTex;
    SDL_Texture*        mBtnResumeTex;
    SDL_Texture*        mBtnExitTex;
	SDL_Texture*        mBackgroundTex;

    Button*             mBtnNew;
    Button*             mBtnResume;
    Button*             mBtnExit;

public:
                        MainMenu(Game* game);
                        ~MainMenu();

    void                Initialize() override;
    void                Shutdown() override;

    void                Update(float dt) override;
    void                Draw(float dt) override;

    void		        OnKeyDown(const SDL_KeyboardEvent& kbe) override;
    void		        OnMouseDown(const SDL_MouseButtonEvent& mbe) override;
};

#endif
