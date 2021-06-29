#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL.h>

#include "Entity.h"
#include "EnemyConstants.h"

class Missile;

enum EnemyState
{
    ENEMY_INIT,
    ENEMY_PATROL,
    ENEMY_ATTACK,
    ENEMY_FLEE,
	ENEMY_HOVER
};

enum EnemyType
{
	ENEMY_TYPE_BEAST,
	ENEMY_TYPE_HUMAN,
	ENEMY_TYPE_AMPHIBIAN,
	ENEMY_TYPE_DEMON,
	ENEMY_TYPE_INSECT
};

class Enemy : public Entity
{
    EnemyState              mState;

    float                   mDefaultSpeed;
    float                   mSpeedScale;

    float                   mDesiredAngle;

    Entity*                 mTarget;            // <-- don't set this member directly; always use SetTarget method
    float                   mNextShootTime;

    float                   mPatrolTurnSpeed;
    float                   mPatrolThinkTime;

public:
							Enemy(SDL_Texture* tex);
                            ~Enemy();

    void                    SetSpeedScale(float s);

    void                    SetAngle(float deg) override;

    void                    SetState(EnemyState newState);

    void                    OnMissileImpact(Missile* missile);

    void                    SetTarget(Entity* target);

	void					Update(float dt) override;

    void                    Draw(SDL_Renderer* renderer, Camera* camera, SDL_RendererFlip flipType = SDL_FLIP_NONE) const override;
};


inline void Enemy::SetAngle(float deg)
{
    Entity::SetAngle(deg);      // call base class method
    mDesiredAngle = deg;        // also set desired angle
}

#endif
