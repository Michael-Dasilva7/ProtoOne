#include "Player.h"
#include "Game.h"
#include "Camera.h"
#include "GlobalsAreBad.h"

#include <string>
#include <iostream>
#include "PlayerConstants.h"

class Texture;

Player::Player(SDL_Texture* tex)
	: Entity(tex)
	, mMoveSpeed(2.0f)   // pixels per second
	, mMoveSpeedScale(1.0f)
	, mPlayerHealth(PlayerConstants::STARTING_HEALTH) //entity has health vbalue....use that???? probably yes unless player health should be diff by why...
{
	SetState(STANDING);
	mDirection = DOWN;
}

Player::Player(SDL_Texture* tex, int numCells, float duration, bool loopable)
	: Entity(tex, numCells, duration, loopable)
	, mMoveSpeed(4)   // pixels per second
	, mMoveSpeedScale(1.0f)
	, mPlayerHealth(PlayerConstants::STARTING_HEALTH)
{
	SetState(STANDING);
	mDirection = DOWN;
}

void Player::InitializePlayerTextures() {
}

void Player::ClearPlayerTextures()
{
}

void Player::Update(float dt)
{
	Game* game = GetGame();

	//determine direction based on velocity

	if (mVelocity.y < -0.1f) mDirection = UP;
	if (mVelocity.y > 0.1f) mDirection = DOWN;
	if (mVelocity.x < -0.1f) mDirection = LEFT;
	if (mVelocity.x > 0.1f) mDirection = RIGHT;

	//check if standing and thats how i can revert the sprite to a normal position!!!
	//if absolute velocity == 0 then im on sprite 0 based on direction

	if (mCurrentState == DAMAGED) {
		Damaged(50);
		//End game!  
		if (mPlayerHealth <= 0) {
			game->EnterMainMenu();
		}
	}
	if (mContinueColorChange) {
		if (mColorChange->IncrementRedValue_WithLimit(5, 255)) {
			mContinueColorChange = false;
		}
	}

	if (mCurrentState == RUNNING) {
		if (mRunUpTexture != nullptr ) {
			mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;
			//TODO: create asset constant files with actual values(i.e.
			// 1.  Duration of Animation from start to finish in seconds
			// 2.  isLoopable; Do we loop the animation or stop after one?
			// 3.  isFlipped;  Do we flip the animation/texture?)
			if (mDirection == UP) {
				setAnimation(mRunUpTexture, 4, 0.4, true);
			}
			else if (mDirection == DOWN) {
				setAnimation(mRunDownTexture, 4, 0.3, true);
			}
			else if (mDirection == LEFT) {
				setAnimation(mRunLeftTexture, 3, 0.4, true);
			}
			else if (mDirection == RIGHT) {
				setAnimation(mRunRightTexture, 3, 0.3, true, SDL_FLIP_HORIZONTAL);
			}
		}

	}
	else if (mCurrentState == WALKING) {
		mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
		//mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
		//TODO: create asset constant files with actual values(i.e.
		// 1.  Duration of Animation from start to finish in seconds
		// 2.  isLoopable; Do we loop the animation or stop after one?
		// 3.  isFlipped;  Do we flip the animation/texture?)
		if (mDirection == UP) {
			setAnimation(mWalkUpTexture, 4, 0.6f, true);
		}
		else if (mDirection == DOWN) {
			setAnimation(mWalkDownTexture, 4, 0.6f, true);
		}
		else if (mDirection == LEFT) {
			setAnimation(mWalkLeftTexture, 4, 1.0f, true);
		}
		else if (mDirection == RIGHT) {
			setAnimation(mWalkRightTexture, 4, 1.0f, true, SDL_FLIP_HORIZONTAL);
		}
	}
	
	if (mVelocity.x == 0.0f && mVelocity.y == 0.0f) mCurrentState = STANDING;
	
	if (mCurrentState != STANDING) {
		Entity::mCurrentAnimation->AddTime(dt);
	} else {
		Entity::mCurrentAnimation->Reset();
	}

	if (!mInCutscene) {
		mCenterPos += mVelocity * mMoveSpeedScale;
	}
	
}





void Player::Draw(SDL_Renderer* renderer, Camera* camera, SDL_RendererFlip flipType) const
{
	//
	// Draw local axes, if visualization enabled
	//
	if (g_EnableDebugVisualization) {
		Vec2 fwdDir = ForwardDir();
		Vec2 rightDir = Vec2(-fwdDir.y, fwdDir.x);

		float fwdAxisLen = 150;
		float rightAxisLen = 100;

		// compute position of forward axis tip (in world space)
		Vec2 forwardTipPos = mCenterPos + fwdAxisLen * fwdDir;
		Vec2 rightTipPos = mCenterPos + rightAxisLen * rightDir;

		// translate axis points to screen space (SDL_RenderDrawLine needs screen-space coordinates)
		SDL_Point scrCenterPos = camera->WorldToScreen(mCenterPos);
		SDL_Point scrForwardPos = camera->WorldToScreen(forwardTipPos);
		SDL_Point scrRightPos = camera->WorldToScreen(rightTipPos);

		// SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, scrCenterPos.x, scrCenterPos.y, scrForwardPos.x, scrForwardPos.y);

		// draw right axis
		// SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderDrawLine(renderer, scrCenterPos.x, scrCenterPos.y, scrRightPos.x, scrRightPos.y);
	}
	//COLOR THE PLAYER TEXTURE
	//mColorChange->Draw(mTex);

	// call base method to draw textured sprite
	Entity::Draw(renderer, camera);

}

void Player::Damaged(int damageAmount)//damagesource argument in the future. perhaps based on enemy type
{
	//in the future, change effect based on source
	//here we awnna do calculations based on defence, other effects, etcs...
	//maybe even move it out somewhere if more tha njust players have to calculate these effects. like monsters do too in battle  
	std::cout << "damaged!" << std::endl;
	mPlayerHealth -= damageAmount;

	if (mPlayerHealth < 0) {
		mPlayerHealth = 0;
	}

	std::cout << "player health: " + std::to_string(mPlayerHealth) << std::endl;
	mContinueColorChange = true;
	mColorChange->ResetColors();
	Player::SetState(mPreviousState);
}

void Player::PlayerDamaged(float damageAmount) {
	//SetState(Damaged)
}

void Player::SetState(mState newState)
{
	if (mCurrentState == newState) {
		if (mCurrentState == Player::DAMAGED) {
			//set damage counter to 0 cause we want to reset animation 
		}
		else {
			return;
		}
	}
	if (newState == Player::DAMAGED) {
		mContinueColorChange = true;
	}
	mPreviousState = mCurrentState;
	mCurrentState = newState;
}

//bool Player::CheckAndSetDirection(Player::Direction dir)
//{
//	return false;
//}


//bool Player::CheckAndSetDirection(Player::CheckAndSetDirection dir) {
//	bool changeDir = false;
//	if (mDirection != dir) {
//		mDirection = dir;
//		changeDir = true;
//	}
//	return changeDir;
//}

//Vec2 Player::moveUp(Vec2& movementLocation, float deltaTime, bool run) {
//	
//		if (run) {
//			if (mRunUpTexture != nullptr) {
//				if (mDirection != UP_RUN) {				
//					mDirection = UP_RUN;
//					this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;
//					//TODO: create asset constant files with actual values(i.e.
//					// 1.  Duration of Animation from start to finish in seconds
//					// 2.  isLoopable; Do we loop the animation or stop after one?
//					// 3.  isFlipped;  Do we flip the animation/texture?)
//					setAnimation(mRunUpTexture, 4, 0.4, true);
//				}
//			}
//		} else {
//			if (mWalkUpTexture != nullptr) {
//				if (mDirection != UP) {
//					mDirection = UP_WALK;
//					this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
//					//TODO: create asset constant files with actual values(i.e.
//					// 1.  Duration of Animation from start to finish in seconds
//					// 2.  isLoopable; Do we loop the animation or stop after one?
//					// 3.  isFlipped;  Do we flip the animation/texture?)
//					setAnimation(mWalkUpTexture, 4, 1, true);
//				}
//			}
//		}
//
//		Entity::mCurrentAnimation->AddTime(deltaTime);
//		movementLocation.y -= this->mMoveSpeed * this->mMoveSpeedScale;
//		return movementLocation;
//}

//
//Vec2 Player::moveDown(Vec2& movementLocation, float deltaTime, bool run) {
//	
//	if (run) {
//		if (checkAndSetDirection(DOWN_RUN)) {
//			this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;
//			setAnimation(mRunDownTexture, 4, 0.8, true);
//		}
//	}
//	else{
//		if (checkAndSetDirection(DOWN_WALK)) {
//			this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
//			setAnimation(mWalkDownTexture, 4, 1.3, true);
//		}
//	}	
//
//	Entity::mCurrentAnimation->AddTime(deltaTime);
//	movementLocation.y += this->mMoveSpeed * this->mMoveSpeedScale;
//	return movementLocation;
//} 
//
//Vec2 Player::moveLeft(Vec2& movementLocation, float deltaTime, bool run) {
//	if (run) {
//		if (mDirection != LEFT_RUN) {
//			mDirection = LEFT_RUN;
//			this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;			 			
//			Entity::setAnimation(mRunLeftTexture, 3, 0.3, true);
//		}
//	} else {
//		if (mDirection != LEFT_WALK) {
//			mDirection = LEFT_WALK;
//			this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
//			setAnimation(mWalkLeftTexture, 4, 1, true);
//		}
//	}
//
//	Entity::mCurrentAnimation->AddTime(deltaTime);
//	movementLocation.x -= this->mMoveSpeed * this->mMoveSpeedScale;
//	return movementLocation;	
//}

//Vec2 Player::moveRight(Vec2& movementLocation, float deltaTime, bool run) {
//
//	if (run) {
//		if (mDirection != RIGHT_RUN) {
//			mDirection = RIGHT_RUN;
//			this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;
//			setAnimation(mRunRightTexture, 3, 0.3, true, SDL_FLIP_HORIZONTAL);
//		}
//	}
//	else {
//		if (mDirection != RIGHT_WALK) {
//			mDirection = RIGHT_WALK;
//			this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
//			//we are using the left animation here but its FLIPPED
//			setAnimation(mWalkRightTexture, 4, 1.0, true, SDL_FLIP_HORIZONTAL);
//		}
//	}
//
//	Entity::mCurrentAnimation->AddTime(deltaTime);
//	movementLocation.x += this->mMoveSpeed * this->mMoveSpeedScale;
//	return movementLocation;
//}