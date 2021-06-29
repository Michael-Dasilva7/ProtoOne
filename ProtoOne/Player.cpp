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
	
}

Player::Player(SDL_Texture* tex, int numCells, float duration, bool loopable)
	: Entity(tex, numCells,duration,loopable)
	, mMoveSpeed(4)   // pixels per second
	, mMoveSpeedScale(1.0f)
	, mPlayerHealth(PlayerConstants::STARTING_HEALTH)
{
	SetState(IDLE);
}

//Will need to eventually hav emultiple characters with stats/etc. who moves around and attacks. 
//so will probably need to add a parameter here so we can load whichever character we want. ie. shadow, then get shadow resources and load them where they exist.
// run, walk, attack, etc.... 

//maybe have a initialize battle textures for player parameter
//then have another another method that loads player textures for runnin the map
void Player::InitializePlayerTextures() { 
	//Game* game = GetGame();
	//SDL_Renderer* renderer = game->GetRenderer();	
}

void Player::ClearPlayerTextures()
{
	//for each texture in: resourceManager.
	//player textures
	//ability textures
	//enemy textures
	//SDL_DestroyTexture(mMoveDownTexture);
	//SDL_DestroyTexture(mMoveLeftTexture);
	//SDL_DestroyTexture(mMoveRightTexture);
	//SDL_DestroyTexture(mMoveUpTexture);
	// 

	//SDL_DestroyTexture(mMoveUpTexture);



}

void Player::Update(float dt)
{
	Game* game = GetGame();

	//add a check for when hit, then misDamaged = True and set reset colorValue
	if (mCurrentState == DAMAGED) {
		//TODO: make dmg random(0.01,0.03) within a calculation based on enemy attack and palyer defence/etc....
		Damaged(50);
		//End game! make gameover screen then EnterGameOver(); 
		if (mPlayerHealth <= 0) {
			game->EnterMainMenu();
		}
	}
	if (mContinueColorChange){
		if (mColorChange->IncrementRedValue_WithLimit(5, 255)) {
			mContinueColorChange = false;
		}
	}
	

    /*

    // get our orientation vectors
    Vec2 fwdDir = ForwardDir();
    Vec2 rightDir = Vec2(-fwdDir.y, fwdDir.x);  // vector perpendicular to fwdDir

    // move forward or backward
    if (keys[SDL_SCANCODE_W]) {
        mCenterPos += mMoveSpeed * mMoveSpeedScale * dt * fwdDir;
    }
    if (keys[SDL_SCANCODE_S]) {
        mCenterPos -= mMoveSpeed * mMoveSpeedScale * dt * fwdDir;
    }
    // strafe left or right
    if (keys[SDL_SCANCODE_D]) {
        mCenterPos += mMoveSpeed * mMoveSpeedScale * dt * rightDir;
    }
    if (keys[SDL_SCANCODE_A]) {
        mCenterPos -= mMoveSpeed * mMoveSpeedScale * dt * rightDir;
    }
    */
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

void Player::SetState(PlayerState newState)
{
	if (mCurrentState == newState) {
		if (mCurrentState == Player::DAMAGED) {
			//set damage counter to 0 cause we want to reset animation 
		}
		else {
			return;
		}
	}
	if (newState == Player::DAMAGED){
		mContinueColorChange = true;
		//need to somehow reset animation.........................
	}
	mPreviousState = mCurrentState;
	mCurrentState = newState;	
}

//
//Vec2 Player::moveUp(Vec2 movementLocation, float deltaTime) {
//	if (mDirection != UP) {
//		mDirection = UP;
//		// std::cout << "up Animation Set" << std::endl;
//		// set currentanimation as walk up - is it loaded already or make new one?	
//
//		//create asset constant files with actual values 
//		//durationOfAnimationBeforeRepeat
//		setAnimation(mMoveUpTexture, 4, 0.7, true);
//	}
//	Entity::mCurrentAnimation->AddTime(deltaTime);
//	//have another animation type but for player effects! :D
//
//	movementLocation.y -= 1;
//	//std::cout << "move up" << std::endl;
//	return movementLocation;
//
//}


/*
void Player::setAnimation(PlayerState state, PlayerDirection dir) {
	switch (dir) {

	case PlayerDirection::LEFT_WALK:
		Entity::setAnimation(mMoveUpTexture, 4, 2, true);
		//mGame->mShouldQuit = true;  // set quit flag		
		break;

	case PlayerDirection::LEFT_RUN:
		Entity::setAnimation(mMoveUpTexture, 4, 2, true);
		break;
	}
}
*/

bool Player::checkAndSetDirection(Player::PlayerDirection dir) {
	bool changeDir = false;
	if (mDirection != dir) {
		mDirection = dir;
		changeDir = true;
	}
	return changeDir;
}
 
Vec2 Player::moveUp(Vec2& movementLocation, float deltaTime, bool run) {
	
	if (mWalkUpTexture != nullptr && mRunUpTexture != nullptr) {
		if (run) {
			if (mDirection != UP_RUN) {				
				mDirection = UP_RUN;
				this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;				
				setAnimation(mRunUpTexture, 4, 0.4, true);
			}

		} else {
			if (mDirection != UP_WALK) {
				mDirection = UP_WALK;
				this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
				setAnimation(mWalkUpTexture, 4, 1, true);
			}
		}
		//TODO: create asset constant files with actual values(i.e.
		// 1.  Duration of Animation from start to finish in seconds
		// 2.  isLoopable; Do we loop the animation or stop after one?
		// 3.  isFlipped;  Do we flip the animation/texture?)
	
		Entity::mCurrentAnimation->AddTime(deltaTime);
		//have another animation type but for player effects! :D
		//TODO: change this FROM decrementing by 1(move up) and return the new value, TO adding a value to a vector.
 		movementLocation.y -= this->mMoveSpeed * this->mMoveSpeedScale;

	}
	//cout << "Player X: " << this->mCenterPos.x << endl;
	//cout << "Player Y: " << this->mCenterPos.y << endl;
	return movementLocation;
}


//if i put runspeed here, cons are that its coupled. do
/**/
Vec2 Player::moveDown(Vec2& movementLocation, float deltaTime, bool run) {
	
	if (run) {
		if (checkAndSetDirection(DOWN_RUN)) {
			this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;
			setAnimation(mRunDownTexture, 4, 0.8, true);
			//the answer is how i want to use them in the future. so. at this point. 
			//moving and running is only in battle. but i want them in cutscenes too.
		}
	}
	else{
		if (checkAndSetDirection(DOWN_WALK)) {
			this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
			setAnimation(mWalkDownTexture, 4, 1.3, true);
			//the answer is how i want to use them in the future. so. at this point. 
			//moving and running is only in battle. but i want them in cutscenes too.
		}
	}	

	//cout << "Player X: " << this->mCenterPos.x << endl;
	//cout << "Player Y: " << this->mCenterPos.y << endl;
	Entity::mCurrentAnimation->AddTime(deltaTime);
	movementLocation.y += this->mMoveSpeed * this->mMoveSpeedScale;
	return movementLocation;
} 

Vec2 Player::moveLeft(Vec2& movementLocation, float deltaTime, bool run) {
	if (run) {
		if (mDirection != LEFT_RUN) {
			mDirection = LEFT_RUN;
			this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;			 			
			Entity::setAnimation(mRunLeftTexture, 3, 0.3, true);
		}
	} else {
		if (mDirection != LEFT_WALK) {
			mDirection = LEFT_WALK;
			this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
			setAnimation(mWalkLeftTexture, 4, 1, true);
		}
	}

	cout << "Player X: " << this->mCenterPos.x << endl;
	cout << "Player Y: " << this->mCenterPos.y << endl;
	Entity::mCurrentAnimation->AddTime(deltaTime);
	movementLocation.x -= this->mMoveSpeed * this->mMoveSpeedScale;

	return movementLocation;	
}

Vec2 Player::moveRight(Vec2& movementLocation, float deltaTime, bool run) {

	if (run) {
		if (mDirection != RIGHT_RUN) {
			mDirection = RIGHT_RUN;
			this->mMoveSpeedScale = PlayerConstants::RUN_SPEED_MODIFYER;
			setAnimation(mRunRightTexture, 3, 0.3, true, SDL_FLIP_HORIZONTAL);
		}
	}
	else {
		if (mDirection != RIGHT_WALK) {
			mDirection = RIGHT_WALK;
			this->mMoveSpeedScale = PlayerConstants::WALK_SPEED_MODIFYER;
			//we are using the left animation here but its FLIPPED
			setAnimation(mWalkRightTexture, 4, 1.0, true, SDL_FLIP_HORIZONTAL);
		}
	}

	cout << "Player X: " << this->mCenterPos.x << endl;
	cout << "Player Y: " << this->mCenterPos.y << endl;
	Entity::mCurrentAnimation->AddTime(deltaTime);

	movementLocation.x += this->mMoveSpeed * this->mMoveSpeedScale;
	 
	return movementLocation;
}