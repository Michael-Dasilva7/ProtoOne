#ifndef BATTLE_H_
#define BATTLE_H_

#include "GameState.h"

#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Camera.h"
#include "Effect.h"



#include <list>
#include <vector>

class Battle : public GameState
{
public: 

	/*
	//battle load:
		load enemies,
		  - load textures
		  - create enemies from new EnemyEncyclopedia class based on type and area passed in
				(we need to pass that in when we start a battle. TODO: put that into gameplay loop to randomly determine # of steps  after a battle til next battle
				 then when battle starts, depending on area, that will determine the monster type, plus the average level of this area. these values should be hard coded
					- players in team passed into battle as well.


		load players
		   - TODO: Inventory and player management system:
		        - status screen, item screen, magic/skill screen

			
	//gameplay phase		
		surprise attack or pre-emptive?
		if surprise then enemy starts first
		if pre-emptive, player turn first
		
		player turn
			arrange players on screen depending on how many

			1 - in the middle
			2 - one player 50 pixels above middle, one 50 pixels below middle
			3 - one player middle, 2 above and below same as 2

			scroll through each char 
			fastest speed or first in order from top to bottom?
	
	
	//gameover if all players hp = 0

	//we will need an enemy attack phase
	//we will nmeed a PLAYer attack phase.
	// 
		//monsters
	skill list
		health
		attack pattern
		mp
		stats...


		//player
		hp
		mp
		skill list
		equipment
	*/


	////I'll do turn based. put in attack and then yuo use it 
	// i want to have a game like saga frontier - but with actionable commands
	// 
	//we need to be able to return to the same place we were walking in gameplay, so we need some sort of gameplay state save
	//void		mTestMe;

	Battle(Game* game, EnemyType monsterType, int level);




	~Battle() {}

	void        Initialize();
	//void        Shutdown() = 0;

	// void        Update(float dt) = 0;
	// void        Draw() = 0;

	// void	    OnWindowResized(int w, int h) {}
	// void		OnKeyDown(const SDL_KeyboardEvent& kbe) {}
	// void		OnKeyUp(const SDL_KeyboardEvent& kbe) {}
	//virtual void		OnMouseDown(const SDL_MouseButtonEvent& mbe) {}
	//virtual void		OnMouseUp(const SDL_MouseButtonEvent& mbe) {}
	//virtual void		OnMouseMotion(const SDL_MouseMotionEvent& mme) {}
};


//
////
////
////#ifndef GAMEPLAY_H_
////#define GAMEPLAY_H_
////
////#include "GameState.h"
////
////#include "Player.h"
////#include "Missile.h"
////#include "Enemy.h"
////#include "Camera.h"
////#include "Effect.h"
//
//#include <list>
//#include <vector>
//
//class Gameplay : public GameState
//{
//public: // <-- HACK
//	Player*				mPlayer;
//
//	std::list<Missile*> mMissiles;
//	std::list<Enemy*>   mEnemies;
//
//	std::list<Entity*>  mMorgue;        // where dead entities get parked until all references are released
//
//	std::list<Effect*>  mEffects;       // temporary animated effects like explosions
//
//	SDL_Texture*        mPlayerTex;
//	SDL_Texture*        mShotTex;
//	SDL_Texture*        mEnemyTex;
//	SDL_Texture*        mBgTex;
//	SDL_Texture*        mExplosionTex;
//
//	int                 mWorldWidth;
//	int                 mWorldHeight;
//
//	bool                mIsActive;
//
//	Camera*             mCamera;
//
//	void                ClipToWorldBounds(Entity* ent);
//
//public:
//	Gameplay(Game* game);
//	~Gameplay();
//
//	bool                IsActive() const { return mIsActive; }
//
//	void                Initialize() override;
//	void                Shutdown() override;
//
//	void                LoadLevel();
//	void                ClearLevel();
//
//	void				Flash();
//
//	void                Update(float dt) override;
//	void                Draw() override;
//
//	void		        OnKeyDown(const SDL_KeyboardEvent& kbe) override;
//	void		        OnKeyUp(const SDL_KeyboardEvent& kbe) override;
//	void                OnMouseDown(const SDL_MouseButtonEvent& mbe) override;
//
//	Player*             GetPlayer() const { return mPlayer; }
//	Camera*             GetCamera() const { return mCamera; }
//
//	int                 GetWorldWidth() const { return mWorldWidth; }
//	int                 GetWorldHeight() const { return mWorldHeight; }
//
//	void                AddMissile(Missile* m) { mMissiles.push_back(m); }
//	void                AddEffect(Effect* e) { mEffects.push_back(e); }
//};
//
//#endif
#endif
