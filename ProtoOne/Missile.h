#ifndef MISSILE_H_
#define MISSILE_H_

#include <SDL.h>

#include "Entity.h"

class Missile : public Entity
{
    float                   mSpeed;
    Entity*                 mShooter;

public:
							Missile(SDL_Texture* tex, Entity* shooter);
                            ~Missile();

    void                    SetSpeed(float speed);

    Entity*                 GetShooter() const      { return mShooter; }

	void					Update(float dt) override;
};

#endif
