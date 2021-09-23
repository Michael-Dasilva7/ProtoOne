#include "NPC.h"



NPC::NPC(SDL_Texture* tex)
	: Entity(tex)
	 
{
}

//talk
//sell armour
//sell weapons
//sell items
//side quests? move around? attack? be part of team?(if s
NPC::NPC(SDL_Texture* tex, int numCells, float duration, const Vec2& pos)
	: Entity(tex, numCells, duration, false)
 
{
}

void NPC::Update(float dt)  
{
	//NPC_INIT,
	//NPC_IDLE,
	//NPC_WANDER,
	switch (mState) {
		case NPC_WANDER:
		{
		}
		case NPC_IDLE:
		{
		}
		case NPC_INIT:
		{
		}
	}

	if (mCurrentAnimation != NULL) {
		Entity::mCurrentAnimation->AddTime(dt);
	}
	
}
	 
void NPC::Draw(SDL_Renderer* renderer, Camera* camera) const
{

	Entity::Draw(renderer, camera);

//	mAnim.Draw(renderer, mCenterPos, camera);
}

void NPC::SetState(NPCState newState)
{
	if (newState == mState) {
		return;
	}

	//switch (newState) {

	//case ENEMY_PATROL:
	//	mDesiredAngle = mAngle;     // just move in a straight line
	//	mSpeedScale = 0.5f;
	//	break;

	//case ENEMY_ATTACK:
	//{
	//	if (g_NoTarget) {
	//		return;             // ignore
	//	}
	//	mNextShootTime = RandomFloat(0.1f, 0.3f);
	//	break;
	//}
	//}

	mState = newState;
}


//move left - change animation, add animation, 