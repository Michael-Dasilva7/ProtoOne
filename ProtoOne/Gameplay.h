#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "GameState.h"

#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Camera.h"
#include "Effect.h"
#include "Actions.h"
#include "Boundary.h"
#include "NPC.h"

#include "StoryScriptConstants.h"

#include "KeyboardHandler.h"

#include <list>
#include <vector>

#include <unordered_map>
#include <map>
class Texture;
//class ResourceManager;
//implements playerstate
class Gameplay : public GameState
{
public: // <-- HACK
	Player*				mPlayer;
	Player*				mPlayer2;

    std::list<Missile*> mMissiles;
    std::list<Enemy*>   mEnemies;
	std::list<NPC*>   mNPCs;
	std::list<Boundary*>   mBoundaries;	
    std::list<Entity*>  mMorgue;        // where dead entities get parked until all references are released
    std::list<Effect*>  mEffects;       // temporary animated effects like explosions

	//used to keep trace of global objects, and looped through to draw layers. when adding a new object, ADD TO THIS.
	//std::map<Entity*, int>  mDrawnObjects;
	std::vector<Entity*> mDrawnObjects;
 

    SDL_Texture*        mPlayerTex;
    SDL_Texture*        mShotTex;
    SDL_Texture*        mEnemyTex;

	int mCounter;
	
	//___________ START BACKGROUNDS __________
	Animation*			mDistantBackground;
	Animation*			mCurrentBackground;
	Animation*			mForeground;
	Animation*			mMidBackground;

	SDL_Texture*        mBgTexFront;
	SDL_Texture*        mBgTexBack; 
	SDL_Texture*        mDesertBackground;	
	//___________ END BACKGROUNDS __________

	//___________ START FIGARO EFFECTS __________
	Effect*        mFigaroWindMill;
	SDL_Texture*   mWindMillTex;	
	//___________ END FIGARO EFFECTS __________
	
	int mTempBoundaryX;
	int mTempBoundaryY;
	int mTempBoundaryW;
	int mTempBoundaryH;

    SDL_Texture*        mExplosionTex;
	SDL_Texture*        mTextImage;
	SDL_Texture*		mTextBoxFF6;	
	SDL_Texture*		Text;
	 
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
	void				OnMouseUp(const SDL_MouseButtonEvent& mbe) override;

    Player*             GetPlayer() const       { return mPlayer; }
    Camera*             GetCamera() const       { return mCamera; }

    int                 GetWorldWidth() const   { return mWorldWidth; }
    int                 GetWorldHeight() const  { return mWorldHeight; }

    void                AddMissile(Missile* m)  { mMissiles.push_back(m); }
    void                AddEffect(Effect* e)    { mEffects.push_back(e); }

	void	            OnWindowResized(int w, int h) override;

	void				CheckCollisionWithBoundary(Boundary* b, Entity* e);
	void				CheckCollisionWithNPC(Entity* n, Entity* e);

	list<Entity*>       MergeLists(list<Entity*> l1, list<Entity*> l2);
};

#endif
