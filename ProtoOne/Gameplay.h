#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "GameState.h"

#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Camera.h"
#include "Effect.h"
#include "Actions.h"

#include "StoryScriptConstants.h"

#include "KeyboardHandler.h"

#include <list>
#include <vector>

#include <unordered_map>
class Texture;
//class ResourceManager;

class Gameplay : public GameState
{
public: // <-- HACK
	Player*				mPlayer;
	Player*				mPlayer2;

    std::list<Missile*> mMissiles;

    std::list<Enemy*>   mEnemies;

    std::list<Entity*>  mMorgue;        // where dead entities get parked until all references are released

    std::list<Effect*>  mEffects;       // temporary animated effects like explosions

    SDL_Texture*        mPlayerTex;
    SDL_Texture*        mShotTex;
    SDL_Texture*        mEnemyTex;


	/*
	___________ BACKGROUNDS __________
	*/
	SDL_Texture*        mBgTexFront;
	SDL_Texture*        mBgTexBack;
	SDL_Texture*        mNarsheBackground;
	SDL_Texture*        mNarsheForeground;
	SDL_Texture*        mDesertBackground;
	/*
	___________ BACKGROUNDS __________
	*/

    SDL_Texture*        mExplosionTex;
	SDL_Texture*        mTextImage;
	SDL_Texture*		mTextBoxFF6;	
	SDL_Texture*		
		Text;
	 
	SDL_Rect*		    mCharacters[22];
	//whats the current total count of characters

	//right now im using these in gameplay AND player. thats not cool.... 
	//this is why manager should have it, and i should use manager in player and in gameplay. i can load the textures in gameplay, sure. but 
	/*SDL_Texture*  mMoveLeftTexture;
	SDL_Texture*  mMoveRightTexture;
	SDL_Texture*  mMoveUpTexture;
	SDL_Texture*  mMoveDownTexture;
	SDL_Texture*  mRunLeftTexture;
*/
	SDL_Renderer*	 mGameRenderer;
	//ResourceManager* mResourceManager;
	//when loading a texture, call it and say playerleft
	//it will call load texture and pass a reference to that texture.\

	//then it needs to destry it, but how does it know when? when we tell it...

    int                 mWorldWidth;
    int                 mWorldHeight;

    bool                mIsActive;

    Camera*             mCamera;

    void                ClipToWorldBounds(Entity* ent);

	aScriptProcessor    mScriptProcessor_Effects;
	aScriptProcessor    mScriptProcessor_CharacterMovements;

	KeyboardHandler 	mGameplayKeyboardHandler;

	bool				mDialogueMode;

public:
                        Gameplay(Game* game, Sound* sound);
                        ~Gameplay();

    bool                IsActive() const        { return mIsActive; }

    void                Initialize() override;
    void                Shutdown() override;

    void                LoadLevel();
    void                ClearLevel();	 

	void                Update(float dt) override;
    void                Draw(float dt) override;

    void		        OnKeyDown(const SDL_KeyboardEvent& kbe) override;
    void		        OnKeyUp(const SDL_KeyboardEvent& kbe) override;
    void                OnMouseDown(const SDL_MouseButtonEvent& mbe) override;

    Player*             GetPlayer() const       { return mPlayer; }
    Camera*             GetCamera() const       { return mCamera; }

    int                 GetWorldWidth() const   { return mWorldWidth; }
    int                 GetWorldHeight() const  { return mWorldHeight; }

    void                AddMissile(Missile* m)  { mMissiles.push_back(m); }
    void                AddEffect(Effect* e)    { mEffects.push_back(e); }

	void	            OnWindowResized(int w, int h) override {  }    					 

};

#endif
