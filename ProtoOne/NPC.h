#ifndef NPC_H_
#define NPC_H_

#include "Animation.h"
#include <string>

//we REALLY need to add this to the entity..or a middle class if we need many more of these classes that use this...
enum class  Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};


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
	 
	Direction					mDirection;

	NPCState					mState;

	NPC(SDL_Texture* tex);
	
	NPC(SDL_Texture* tex, int numCells, float duration, const Vec2& pos);
	NPC(SDL_Texture* tex, int numCells, float duration, bool loopable, int hitBoxW, int hitBoxH);

	void                        Update(float dt);	 
	float						mPatrolThinkTime;
	void                        Draw(SDL_Renderer* renderer, Camera* camera) const;

	void						SetState(NPCState newState);
	void						SetIdleDirection(Direction direction);

	std::string					mDialogue;
	std::string					mName;

	bool						mTalkable;
};

#endif
