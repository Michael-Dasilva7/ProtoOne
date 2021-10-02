#include "NPC.h"



NPC::NPC(SDL_Texture* tex)
	: Entity(tex),
	mPatrolThinkTime(0.0f),
	mState(NPC_IDLE)
{
}

//Dont forget to populate the mDialogue class variable so that they say something when spoken to(loop is in gameplay)
//talk
//sell armour
//sell weapons
//sell items
//side quests? move around? attack? be part of team?(if s
NPC::NPC(SDL_Texture* tex, int numCells, float duration, const Vec2& pos)
	: Entity(tex, numCells, duration, true),
	mPatrolThinkTime(0.0f),
	mState(NPC_IDLE)
{
}

void NPC::Update(float dt)  
{

	
	Vec2 fwd = ForwardDir();

	switch (mState) {

		case NPC_WANDER:
		{
			////pick random direction
			//if (t >= mPatrolThinkTime) {
			//	mPatrolTurnSpeed = RandomFloat(-90.0f, 90.0f);
			//	mPatrolThinkTime = t + RandomFloat(1.0f, 3.0f);
			//}
			Vec2 fwd = ForwardDir();
			//*******************************************************************************
			//change fwd every so often to point in a different direction! like enemy when
			//bounding off the walls. so like after 4 seconds, switch direction!!
			//*******************************************************************************
			SetForwardDir(fwd);


		}
		case NPC_IDLE:
		{

		}
		case NPC_INIT:
		{
		}
	}


	if (fwd.y < -0.1f) mDirection = Direction::UP;
	if (fwd.y > 0.1f) mDirection = Direction::DOWN;
	if (fwd.x < -0.1f) mDirection = Direction::LEFT;
	if (fwd.x > 0.1f) mDirection = Direction::RIGHT;

	// testing, testing, testing
	//different animations have different textures. some enemies dont run...some do
	//need to make this customado.
	// ideas:
		//1. Make the texture based on the EnemyType? 
		//2. then we would need to add a new if statemnet each one. which is fine, unless we see lots of future configuration happenining here
			//then in that case we should make a inheiritance solution.
	//if (mDirection == UP) {
	//	setAnimation(mRunUpTexture, 3, 0.4, true);
	//}
	//else if (mDirection == DOWN) {
	//	setAnimation(mRunDownTexture, 3, 0.3, true);
	//}
	//else if (mDirection == LEFT) {
	//	setAnimation(mRunLeftTexture, 3, 0.4, true);
	//}
	//else if (mDirection == RIGHT) {
	//	setAnimation(mRunRightTexture, 3, 0.3, true, SDL_FLIP_HORIZONTAL);
	//}

	if (mCurrentAnimation != NULL) {
		Entity::mCurrentAnimation->AddTime(dt);
	}
	
}
	 
void NPC::Draw(SDL_Renderer* renderer, Camera* camera) const
{

	Entity::Draw(renderer, camera);

//	mAnim.Draw(renderer, mCenterPos, camera);
}

void NPC::SetIdleDirection(Direction direction) {
	mDirection = direction;
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