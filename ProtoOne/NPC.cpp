#include "NPC.h"

NPC::NPC(SDL_Texture* tex, int numCells, float duration, const Vec2& pos)
	: mAnim(tex, numCells, duration, false)
	, mCenterPos(pos)
{
}

void NPC::Draw(SDL_Renderer* renderer, Camera* camera) const
{
	mAnim.Draw(renderer, mCenterPos, camera);
}

//move left - change animation, add animation, 