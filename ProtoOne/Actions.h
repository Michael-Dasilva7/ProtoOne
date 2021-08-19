#pragma once

#include <iostream> 

using namespace std;

#include <list>
#include <unordered_map>
#include "Player.h"
#include "Animation.h"
#include "Sound.h"

class Action {

public:

	Action() {}
	virtual ~Action() {}
	bool isDone = false;
	bool isStarted = false;
	virtual void Start() {};
	virtual void Start(int width, int height) {};
	virtual void Update(float elapsedTime) {};
	virtual void Update(SDL_Renderer* renderer) {};
	virtual void Draw(SDL_Renderer* renderer) {};

};
//maintains access to the script processor list
class aScriptProcessor
{
public:

	bool userControlEnabled = false;

	aScriptProcessor();

	std::list<Action*> m_listActions;

	void AddAction(Action *action);

	void ProcessActions(float fElapsedTime);

};


class Lerp {
public:
	float mTimeSoFar;
	float mDuration;
	Vec2 mStart;
	Vec2 mEnd;

	Vec2 mPosition;

	Lerp(Vec2 start, Vec2 end, Vec2 position, float duration) {
		mStart = start;
		mEnd = end;
		mPosition = position;
		mDuration = duration;
		mTimeSoFar = 0.0f;
	}

	  bool update(float timesofar)
	{
		mTimeSoFar += timesofar;
		float t = mTimeSoFar / mDuration;
		if (t > 1.0f) t = 1.0f;

		mPosition.x = ((mEnd.x - mStart.x) * t + mStart.x);
		mPosition.y = ((mEnd.y - mStart.y) * t + mStart.y);

		//mMoveableObject->mVelocity.x = (mEnd.x - mStart.x) / mDuration;
		//mMoveableObject->mVelocity.y = (mEnd.y - mStart.y) / mDuration;

		if (mTimeSoFar >= mDuration) {
			mPosition.x = mEnd.x;
			mPosition.y = mEnd.y;

			return true;
			//if (mEndCutscene) mMoveableObject->mInCutscene = false;
		}
		else {
			return false;
		}
	}
};


class aAction_BigBang : public Action
{

public:
	aAction_BigBang(Entity* object, Vec2 target, SDL_Texture* fireball, SDL_Texture* fireball2, Animation* explosion, Animation* cast, SDL_Renderer* renderer, Camera* cam, Sound* sound);

	void			 Start() override;
	void			 Update(float fElapsedTime) override;

	float			 mVel;
	Vec2			 mDir;
	bool			 mAnimate;
	bool			 EndCutscene;

	Entity*			 mPlayer;
	Vec2			 mTarget;
	Vec2			 mTargetCenter;
	bool			 mPhase1;
	bool			 mPhase2;
	bool			 mPhase3;
	bool			 mPhase4;

	SDL_Texture*     mFireball;
	SDL_Texture*     mFireball2;
	SDL_Rect         mFireballRect;
	SDL_Rect		 mFireball2Rect;
	SDL_Rect		 mScreenExplosion;


	Lerp*			 mCurrentLerp;

	Animation*		 mChant;
	Animation*		 mExplosion;
	Animation*		 mCast;

	Camera*			 mCamera;
	SDL_Renderer*	 mRenderer;
	Sound*			 mSound;

private:
	Entity*			 mMoveableObject;
	Vec2			 mStart;
	Vec2			 mEnd;
	float			 mTimeSoFar;
	float			 mDuration;

};


class aAction_MoveTo : public Action
{

public:
	aAction_MoveTo(Entity* object, Vec2 end, float duration, bool endCutscene = false, bool animate = true);

	void Start() override;
	void Update(float fElapsedTime) override;

	float mVel;
	Vec2  mDir;
	bool  mAnimate;
	bool  mEndCutscene;
private:
	Entity*  mMoveableObject;
	Vec2	 mStart;
	Vec2	 mEnd;
	float    mTimeSoFar;
	float    mDuration;

};

class aAction_FadeIn : public Action
{

public:
	aAction_FadeIn(int w, int h, float duration, SDL_Renderer* renderer, int fromR, int fromG, int fromB, int toR, int toG, int toB, int fromOpacity, int toOpacity);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	SDL_Rect			mFadeRect;
	float				mDecrementer;
	float				mOpacity;

	float				exponent;
	SDL_Renderer*		mRenderer;

	int16_t				mW;
	int16_t				mH;

	float				mFR;
	float				mFG;
	float				mFB;
	float				mTR;
	float				mTG;
	float				mTB;
	float				mFO;
	float				mTO;

	float			mTimeSoFar;
	float			mDuration;
};

class aAction_Delay : public Action {

private:
	unsigned short int durationInMilliseconds;
	unsigned short int mOriginalDuration;
public:
	aAction_Delay();
	aAction_Delay(unsigned short int durationInMilliseconds);
	void Restart();
	void Update(float elapsedTime) override;
};

class aAction_PanCamera : public Action {

private:
	unsigned short int durationInMilliseconds;
	unsigned short int mOriginalDuration;
public:
	// I want the camera to slowly move to the location specified.
	/*
	the challange here is that the camaera updates every frame in gameplay
		it looks at whatever is set to look at. so either i need to unlook at the beginning...then look at the end
		but if i do that, the camera will jump when i go back....
		option 1:
			if no target is set
		from
			to
			update gets next lerp position,
	...so what i should do, is take a from and a to...so i can do a couple in a row and go back to the original entity
	*/
	Entity*			mOriginalEntity;
	Camera*			mCamera;

	Vec2			mStart;
	Vec2			mEnd;
	float			mTimeSoFar;
	float			mDuration;
	bool			mLastCameraMovement;

	aAction_PanCamera(Vec2 panCameraFrom, Vec2 panCameraTo, Camera* camera, float durationOfCameraPan, bool lastCameraMovement = false);

	//void Start() override;

	void Update(float elapsedTime) override;
};

class aAction_ChangeAnimation : public Action
{

public:
	aAction_ChangeAnimation(Entity* object, SDL_Texture* t[], int delayInMilliseconds);
	void Start() override;
	void Update(float fElapsedTime) override;

	Entity*  mEntity;
	aAction_Delay* mDelayAfterFirstAnimation;
	SDL_Texture* mTextures[];

};

class aAction_Dialogue : public Action {
public:
	//MAKE SURE TO CLEAN ME UP END OF GAMEPLAY OR WHEREVER
	SDL_Renderer* mRenderer;
	SDL_Texture* mTextBox;
	SDL_Texture* mDialogue;
	SDL_Texture* texCharacterTexture;
	SDL_Rect mDestRectForTextBox;

	//TODO : SEE IF I CAN ACCCESS RESOURCEMANAGER DFROM HERE
	//std::unordered_map<int, SDL_Rect*>  ResourceManager::characterTexturePtrs;
	std::unordered_map<int, SDL_Rect*>* mMappingToCharacterTexture;
	//currently 
	SDL_Rect* locationOfCharacters[87];
	int mXPos;
	int mYPos;
	int mTextBoxWidth;
	int mTextBoxHeight;

	short int mXOffsetFromTextBox;
	short int mNextXPosIncrement;

	int mDialogueX;
	int mCurrentDialogueCharacter;
	int mCurrentDialogueWord;
	int mStartingDialogueCharacter;
	int mNextStartingDialogueCharacter;

	short int mLineNumber;

	bool mAtEndOfTextBox;
	bool mTextComplete;

	bool mForceSkipLine;
	bool mForceSkipDialogueProgression;
	bool mCheckForProgressDelay;

	int mFlashingCounter;
	SDL_Event mE;
	std::vector<std::string> mWordTokens;

	aAction_Delay* mTextDelayForNextCharacter;
	aAction_Delay* mDelayToProgressDialogue;

	SDL_Texture* characterList[1];

	string mWrittenDialogue;
	aAction_Dialogue(
		float x,
		float y,
		float width,
		float height,
		SDL_Texture* texDialogueBox,
		SDL_Texture* texCharacterTexture,
		unordered_map<int, SDL_Rect*>* listOfRects,
		SDL_Renderer* renderer,
		string writtenDialogue,
		unsigned short int textDelayInMilliseconds,
		SDL_Event e,
		unsigned short int delayToProgressDialogueMilliseconds = 0,//optional
		bool forceSkipDialogueProgression = false //optional

	);

	void Update(float elapsedTime) override;
	void CreateRectList() {}

};



