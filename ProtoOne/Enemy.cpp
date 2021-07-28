#include <iostream>

#include "Enemy.h"
#include "Game.h"
#include "Gameplay.h"
#include "Util.h"
#include "GlobalsAreBad.h"
#include "Missile.h"

Enemy::Enemy(SDL_Texture* tex)
	: Entity(tex)
    , mState(ENEMY_INIT)
    , mDefaultSpeed(200)   // in pixels per second
	, mSpeedScale(1.0f)
    , mDesiredAngle(mAngle)
    , mTarget(NULL)
    , mNextShootTime(0.0f)
    , mPatrolTurnSpeed(0.0f)
    , mPatrolThinkTime(0.0f)
	
{
    SetState(ENEMY_PATROL);
	mDirection = DOWN;

}

Enemy::Enemy(SDL_Texture* tex, int numCells, float duration, bool loopable)
	: Entity(tex, numCells, duration, loopable)
	, mState(ENEMY_INIT)
	, mDefaultSpeed(200)   // in pixels per second
	, mSpeedScale(1.0f)
	, mDesiredAngle(mAngle)
	, mTarget(NULL)
	, mNextShootTime(0.0f)
	, mPatrolTurnSpeed(0.0f)
	, mPatrolThinkTime(0.0f)

{
	SetState(ENEMY_PATROL);
	mDirection = DOWN;

} 

Enemy::~Enemy()
{
    std::cout << "Enemy destroyed" << std::endl;
}

void Enemy::SetSpeedScale(float s)
{
    mSpeedScale = s;
}

void Enemy::SetState(EnemyState newState)
{
    if (newState == mState) {
        return;
    }

    switch (newState) {

    case ENEMY_PATROL:
        mDesiredAngle = mAngle;     // just move in a straight line
        mSpeedScale = 0.5f;
        break;

    case ENEMY_ATTACK:
        {
            if (g_NoTarget) {
                return;             // ignore
            }
            mNextShootTime = RandomFloat(0.1f, 0.3f);
            break;
        }
    }

    mState = newState;
}

void Enemy::Update(float dt)
{
    Game* game = GetGame();

    float t = game->GetTime();

    //SetTarget(game->GetGameplayState()->GetPlayer());

    switch (mState) {

    case ENEMY_PATROL:
        {
            if (!g_NoTarget && mTarget && Dist(mCenterPos, mTarget->Center()) < 300) {
                SetState(ENEMY_ATTACK);
                break;
            }

            if (t >= mPatrolThinkTime) {
                mPatrolTurnSpeed = RandomFloat(-90.0f, 90.0f);
                mPatrolThinkTime = t + RandomFloat(1.0f, 3.0f);
            }

            mAngle += dt * mPatrolTurnSpeed;

            // check for collisions with world bounds
            float w = (float)game->GetGameplayState()->GetWorldWidth();
            float h = (float)game->GetGameplayState()->GetWorldHeight();

            // bounce off of world boundaries
            Vec2 fwd = ForwardDir();
            if (fwd.x > 0 && Right() >= w) {
                //SetRight(w);
                fwd.x *= -1;
            } else if (fwd.x < 0 && Left() <= 0) {
                //SetLeft(0);
                fwd.x *= -1;
            }
            if (fwd.y > 0 && Bottom() >= h) {
                //SetBottom(h);
                fwd.y *= -1;
            } else if (fwd.y < 0 && Top() <= 0) {
                //SetTop(0);
                fwd.y *= -1;
            }
            SetForwardDir(fwd);

			//change this to be an entity
			//change animation dependong on angle
			if (fwd.y < -0.1f) mDirection = UP;
			if (fwd.y > 0.1f) mDirection = DOWN;
			if (fwd.x < -0.1f) mDirection = LEFT;
			if (fwd.x > 0.1f) mDirection = RIGHT;

			// testing, testing, testing
			//different animations have different textures. some enemies dont run...some do
			//need to make this customado.
			// ideas:
				//1. Make the texture based on the EnemyType? 
				//2. then we would need to add a new if statemnet each one. which is fine, unless we see lots of future configuration happenining here
					//then in that case we should make a inheiritance solution.
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

			if (mCurrentAnimation != NULL){
				Entity::mCurrentAnimation->AddTime(dt);
			}

        }

    case ENEMY_ATTACK:
        {
            if (!mTarget || g_NoTarget) {
                SetState(ENEMY_PATROL);
                break;
            }

            if (mTarget->IsDead()) {
                SetTarget(NULL);
                SetState(ENEMY_PATROL);
                break;
            }

            Vec2 toTarget = mTarget->Center() - mCenterPos;
            
            mDesiredAngle = Rad2Deg(std::atan2(toTarget.y, toTarget.x));

            //std::cout << "Desired angle " << mDesiredAngle << std::endl;
            //std::cout << "Target pos " << mTarget->Center().x << ", " << mTarget->Center().y << std::endl;

            float angle = SignedAngleDiff(mAngle, mDesiredAngle);

            // turn to face target
            float rotSpeed = 360;
            if (std::fabs(angle) > dt * rotSpeed) {
                if (angle > 0) {
                    mAngle += dt * rotSpeed;
                } else {
                    mAngle -= dt * rotSpeed;
                }
            } else {
                mAngle = mDesiredAngle;
            }

            // shoot?
            if (t >= mNextShootTime && std::fabs(mDesiredAngle - mAngle) < 5) {
				Missile* m = new Missile(game->GetGameplayState()->mShotTex, this, 60, 3, true);
                m->SetSpriteAngleCorrection(DIR_UP);
                m->SetCenter(mCenterPos);
                m->SetAngle(mDesiredAngle);
                mNextShootTime = t + RandomFloat(0.2f, 1.0f);
                game->GetGameplayState()->AddMissile(m);
            }

            //________    (Distance to target)          MINUS   The sum of the two targets radius.  ______  This is  so they dont not clip into each other's sprites
            float d = Dist(mCenterPos, mTarget->Center()) - mRadius - mTarget->Radius();

            if (d < 50) {
                // target too close, back up
                mSpeedScale = -1.0f;
            } else if (d < 150) {
                // target within comfy range, stay put
                mSpeedScale = 0.0f;
            } else if (d < 300) {
                // target too far, get closer
                mSpeedScale = 1.0f;
            } else {
                // target out of range
                SetTarget(NULL);
                SetState(ENEMY_PATROL);
            }

            break;
        }
		case ENEMY_HOVER:
		{
						
			//sinve we hover, we need to set our TARGET!! omg..of course...
			if (mCenterPos.y) {

			}
			break;
		}
    }

    // move along the forward/backward axis
    mCenterPos += mDefaultSpeed * mSpeedScale * dt * ForwardDir();

}


void Enemy::Draw(SDL_Renderer* renderer, Camera* camera, SDL_RendererFlip flipType) const
{
    //
    // Draw bounding rect
    //
    if (g_EnableDebugVisualization) {
        SDL_Rect rect;
        rect.x = camera->WorldToScreenX(Left());
        rect.y = camera->WorldToScreenY(Top());
        rect.w = camera->WorldToScreenX(Right()) - rect.x;
        rect.h = camera->WorldToScreenY(Bottom()) - rect.y;
        //rect.w = mTexWidth;
        //rect.h = mTexHeight;
        SDL_Color color = { 0, 0, 0, 255 };
        switch (mState) {
			case ENEMY_PATROL:
				color.g = 128;
				break;
			case ENEMY_ATTACK:
				color.r = 255;
				break;
			case ENEMY_FLEE:
				color.r = 255;
				color.g = 255;
				break;
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &rect);
    }

    // call base method to draw textured sprite
    Entity::Draw(renderer, camera);
}


void Enemy::OnMissileImpact(Missile* missile)
{
    mHealth -= 30;
    if (IsAlive()) {
        SetTarget(missile->GetShooter());
        SetState(ENEMY_ATTACK);
    }
}


void Enemy::SetTarget(Entity* target)
{
    if (mTarget) {
        --mTarget->mRefCount;
    }
    mTarget = target;
    if (mTarget) {
        ++mTarget->mRefCount;
    }
}
