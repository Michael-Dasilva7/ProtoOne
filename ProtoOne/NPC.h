#ifndef NPC_H_
#define NPC_H_

#include "Animation.h"

// a class for temporary animated effects
class NPC
{
	Animation                   mAnim;

	Vec2                        mCenterPos; // position of sprite center in world coordinates

public:
	NPC(SDL_Texture* tex, int numCells, float duration, const Vec2& pos);

	void                        Update(float dt) { mAnim.AddTime(dt); }

	bool                        IsDone() const { return mAnim.IsDone(); }

	void                        Draw(SDL_Renderer* renderer, Camera* camera) const;
};

#endif
