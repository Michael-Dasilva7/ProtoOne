#ifndef NPC_H_
#define NPC_H_

#include "Animation.h"
 

enum NPCState
{
	NPC_INIT,
	NPC_IDLE,
	NPC_WANDER,
};

class NPC : public Entity
{ 
	Vec2                        mCenterPos; // position of sprite center in world coordinates

public:
	 

	NPCState					mState;
	NPC(SDL_Texture* tex);
	NPC(SDL_Texture* tex, int numCells, float duration, const Vec2& pos);
	
	void                        Update(float dt);	 

	void                        Draw(SDL_Renderer* renderer, Camera* camera) const;

	void						SetState(NPCState newState);
};

#endif
