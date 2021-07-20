#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL.h>

#include "Entity.h"
#include "PlayerConstants.h"


class Player : public Entity
{

protected:
    float                   mMoveSpeed;



public:
	enum  mDirection {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum  mState {
		IDLE,
		DAMAGED,
		ATTACKING,
		RUNNING,
		WALKING,
		STANDING
	};
	Player(SDL_Texture* tex);

	Player(SDL_Texture* tex, int numCells, float duration, bool loopable);	//player w/ animation

	void					SetState(mState newState);
	bool					CheckAndSetDirection(mDirection dir);

	mDirection				mDirection;

	mState					mCurrentState;
	mState					mPreviousState;
	 
	SDL_Texture*			mWalkLeftTexture;
	SDL_Texture*			mWalkUpTexture;
	SDL_Texture*			mWalkDownTexture;
	SDL_Texture*			mWalkRightTexture;

	SDL_Texture*			mRunLeftTexture;
	SDL_Texture*			mRunUpTexture;
	SDL_Texture*			mRunDownTexture;
	SDL_Texture*			mRunRightTexture;

	//need a spell texture pointer. and another manager. so i just load spells from that repository as needed
	//maybe just have a current spell pointer here and do logic in game

	void				    InitializePlayerTextures();
	void				    ClearPlayerTextures();

	Vec2					moveUp(Vec2& movementLocation, float deltaTime, bool run);
	Vec2					moveDown(Vec2& movementLocation, float deltaTime, bool run);
	Vec2					moveLeft(Vec2& movementLocation, float deltaTime, bool run);
	Vec2					moveRight(Vec2& movementLocation, float deltaTime, bool run);
	
	//bool					SetPlayerUpDir();///Sets the player direction and animation up 
	bool					SetPlayerDownDir();///Sets the player direction and animation up 
	bool					SetPlayerLeftDir();///Sets the player direction and animation up 
	bool					SetPlayerRightDir();///Sets the player direction and animation up 

	void					setState(mState x) { mCurrentState = x; }

	float					mPlayerHealth;

	bool					mContinueColorChange;

    float                   mMoveSpeedScale;
						 
	void					Update(float dt) override;

    void                    Draw(SDL_Renderer* renderer, Camera* camera, SDL_RendererFlip flipType = SDL_FLIP_NONE) const override;

	void					Damaged(int damageAmount);

	void					PlayerDamaged(float damageAmount);//eventually want to calculate damage based on weaknesses, attacker vs defender
	//void					setAnimation(PlayerState state, PlayerDirection dir);
};

#endif
