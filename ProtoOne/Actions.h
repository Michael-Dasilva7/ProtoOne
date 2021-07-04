#pragma once

#include <iostream> 

using namespace std;

#include <list>
#include <unordered_map>
#include "Player.h"


//class Delay {
//private:
//	unsigned short int mDurationInMilliseconds;
//	bool isDone;
//public:
//	//same as my current ethod just copy it
//	Delay(int durationInMilliseconds) {
//		mDurationInMilliseconds = durationInMilliseconds;
//		isDone = false;
//	}
//
//};
//An interface for an Action
//Examples: 
// 1. Display a Lightning effect
// 2. Move a player to a specified location
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

class aAction_MoveTo : public Action
{

public:
	aAction_MoveTo(Player* object, Vec2 end, float duration);
	void Start() override;
	void Update(float fElapsedTime) override;	

private:
	Player*  mMoveableObject;
	Vec2	 mStart;
	Vec2	 mEnd;

};

class aAction_FadeIn : public Action
{

public:
	// Its basically a black rectange overlaying the screen that gets increasingly opaque
	// MIGHT need a screensize but probably can grab it from wherever the action is (i.e. game load)
	// increment value is determined by the duration. 
	aAction_FadeIn(int w, int h, float duration, SDL_Renderer* renderer, int r, int g, int b);
	void Start() override;
	void Update(float fElapsedTime) override;

	//MAKE A LIST OF RECTANGLES WITH DIFFERNET OPAQUES / COLORS. so you can have a flashing rectangle behind a red one. for spells maybe.

	//also can use this fade in to fade in from black.

	//will need to have two implementations of the processor. one for each layer and call it at the right point in time.

private:
	SDL_Rect   mFadeRect;
	float    mDecrementer;
	float    mOpacity;

	float    exponent;
	SDL_Renderer* mRenderer;
	
	//int16_t	 x;
	//int16_t	 y;
	int16_t  mW;
	int16_t  mH;

	int16_t  mR;
	int16_t  mG;
	int16_t  mB;
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
					unsigned short int delayToProgressDialogueMilliseconds = 0,//optional
					bool forceSkipDialogueProgression = false //optional
					);

	//fixed location for now(later we can pass in a x.y if we need to
	//need to figure out how to tell if a button is pressed and go to the next "page" of text. also need to figure this out how am i gonna page the text?
	
	void Update(float elapsedTime) override;
	void CreateRectList() {}

};