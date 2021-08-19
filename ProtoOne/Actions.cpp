#include "Actions.h"
#include "Player.h"

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "SoundConstants.h"

aScriptProcessor::aScriptProcessor() {
	userControlEnabled = true;
}

void aScriptProcessor::AddAction(Action *action) {
	m_listActions.push_back(action);
}

void aScriptProcessor::ProcessActions(float fElapsedTime) {
	//now we must force actions to specify whether use control is enabled. or else itll spill into each other
	//if (m_listActions.empty()) {
	//	userControlEnabled = true;
	//}
	userControlEnabled = m_listActions.empty();

	if (!m_listActions.empty()) {
		if (!m_listActions.front()->isDone) {
			if (!m_listActions.front()->isStarted) {
				m_listActions.front()->Start();
				m_listActions.front()->isStarted = true;
			}
			else {
				m_listActions.front()->Update(fElapsedTime);
			}
		}
		else {
			delete m_listActions.front();
			m_listActions.pop_front();
		}

	}
}
aAction_ChangeAnimation::aAction_ChangeAnimation(Entity* object, SDL_Texture* t[], int delayInMilliseconds) {
	//if we want to change the textre
	mEntity = object;
	object->setAnimationTexture(t[0]);
	mDelayAfterFirstAnimation = new aAction_Delay(delayInMilliseconds);

	//aScriptProcessor::userControlEnabled = false;
}
void aAction_ChangeAnimation::Start() {

}
void aAction_ChangeAnimation::Update(float dt) {
	//color change
	if (!mDelayAfterFirstAnimation->isDone) {
		mDelayAfterFirstAnimation->Update(dt);
		mEntity->addTimeToAnimation(dt);

	}
	else {
		//change animation
		//mEntity->setAnimationTexture(t[1]);
		isDone = true;
		//if (!mDelayToProgressDialogue->isDone) {
		//	mDelayToProgressDialogue->Update(elapsedTime);
		//}
		//else {
		//	isDone = true;
		//}

	}
}

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!SPRITE MOVEMENT FOR CUTSCENE!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

//int *ptr[MAX];
//
//for (int i = 0; i < MAX; i++) {
//	ptr[i] = &var[i]; // assign the address of integer.
//}
aAction_MoveTo::aAction_MoveTo(Entity* object, Vec2 end, float duration, bool endCutscene, bool animate) {
	mAnimate = animate;
	mMoveableObject = object;
	cout << "mMoveableObject x contructor: " << mMoveableObject->Center().x << endl;
	cout << "mMoveableObject y contructor: " << mMoveableObject->Center().y << endl;
	mMoveableObject->mInCutscene = true;
	mEnd = end;
	mTimeSoFar = 0.0f;
	mDuration = max(duration, 0.001f);//prevent divide by zero later and to cap durations
	//mVel = 2.0f;
	mEndCutscene = endCutscene;
}

void aAction_MoveTo::Start() {

	mStart = mMoveableObject->Center();
	cout << "mMoveableObject x: " << mMoveableObject->Center().x << endl;
	cout << "mMoveableObject y: " << mMoveableObject->Center().y << endl;
	////check direction. if its up, move up. if its down , move down....etc...
	//float degreeofDirection = Rad2Deg(atan2(mDir.y, mDir.x));
	//degreeofDirection = abs(degreeofDirection);

	////x +   >
	////x -   <
	////y +   V
	////Y -   ^

	////0    >
	////90   ^
	////180  <
	////260  V

	//if (degreeofDirection < 60 && degreeofDirection > 25)
	//{
	//	//set direction and change player or entity to move based on direction and update velocity
	//	//mMoveableObject->mVel;
	//}
	//else if (degreeofDirection >= 60 && degreeofDirection <= 100)
	//{
	//	//mMoveableObject->moveUp(mStart, 0, false);
	//}
	//else if (degreeofDirection > 100 && degreeofDirection <= 220)
	//{
	//	//mMoveableObject->moveLeft(mStart, 0, false);
	//}
	//else if (degreeofDirection > 220 && degreeofDirection <= 320)
	//{
	//	//mMoveableObject->moveDown(mStart, 0, false);
	//}

}
void aAction_MoveTo::Update(float dt) {
	mTimeSoFar += dt;
	float t = mTimeSoFar / mDuration;
	if (t > 1.0f) t = 1.0f;

	mMoveableObject->SetCenter((mEnd - mStart) * t + mStart);
	mMoveableObject->mVelocity.x = (mEnd.x - mStart.x) / mDuration;
	mMoveableObject->mVelocity.y = (mEnd.y - mStart.y) / mDuration;

	if (mTimeSoFar >= mDuration) {
		mMoveableObject->SetCenter(mEnd);
		mMoveableObject->mVelocity.x = 0.0f;
		mMoveableObject->mVelocity.y = 0.0f;

		isDone = true;
		if (mEndCutscene) mMoveableObject->mInCutscene = false;
	}

}


/*

The higher the duration, the slower fade

Opacity:
	For Fade in:
		From: 255
		To:   0
	For Fade Out:
		From: 0
		To:   255


	Color Combinations:
		white:  255,255,255  (i.e. lightning)
		black:  0,0,0
		other:  0,0,0
		Red:    255,0,0
		Green:  0,255,0
		Blue    0,0,255
		orange: 0,0,0

*/
aAction_FadeIn::aAction_FadeIn(int w, int h, float duration, SDL_Renderer* renderer, int fromR, int fromG, int fromB, int toR, int toG, int toB, int fromOpacity, int toOpacity) {
	mW = w;
	mH = h;

	mFR = fromR;
	mFG = fromG;
	mFB = fromB;

	mTR = toR;
	mTG = toG;
	mTB = toB;

	mFO = fromOpacity;
	mTO = toOpacity;

	mDuration = duration;
	//mDecrementer = 1000 / duration;
	mRenderer = renderer;
	mTimeSoFar = 0.0f;
	//mOpacity = 255;
	//mFadeRect.x = 0;
	//mFadeRect.y = 0;
	//mFadeRect.w = mW;
	//mFadeRect.h = mH;

}

void aAction_FadeIn::Start() {
	//mOpacity = 255;
	mFadeRect.x = 0;
	mFadeRect.y = 0;
	mFadeRect.w = mW;
	mFadeRect.h = mH;
}

void aAction_FadeIn::Update(float fElapsedTime) {

	mTimeSoFar += fElapsedTime;
	float t = mTimeSoFar / mDuration;
	if (t > 1.0f) t = 1.0f;

	SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
	//the code below is lerping between the start and end RGB and A values to slowly transition between the two
	SDL_SetRenderDrawColor(mRenderer, (mFR - (mFR - mTR) * t), mFG - ((mFG - mTG) * t), mFB - ((mFB - mTB) * t), mFO - ((mFO - mTO) * t));

	//TODO: make a change to increase or decrease the duration variably to speed up or slowdown the fade
	SDL_RenderFillRect(mRenderer, &mFadeRect);

	if (mTimeSoFar >= mDuration) {
		isDone = true;
		//delete &mFadeRect;
	}
}

aAction_Delay::aAction_Delay(unsigned short int durationInMilliseconds) {
	this->mOriginalDuration = durationInMilliseconds;
	this->durationInMilliseconds = durationInMilliseconds;
}
void aAction_Delay::Update(float elapsedTime) {

	if (durationInMilliseconds <= (elapsedTime * 1000)) {
		//if you subtract too big of an amount, the value will underflow and may potentially never be less than 0.
		//so im doing this logic isntead to check if the amount subtracted from the previous call is 
		// greater than the number. if so, dont subtract
		isDone = true;
	}
	else {
		durationInMilliseconds -= (elapsedTime * 1000);
	}
}
void aAction_Delay::Restart() {
	isDone = false;
	durationInMilliseconds = mOriginalDuration;
}

/*
1. Must specify the same # of movements in "panCameraFrom" and "panCameraTo"
**************START WARNING******************
2. if specifying multiple camera pan movements, need to specific "true" as the last boolean or the camera will not look at the user
************** END  WARNING******************
*/
aAction_PanCamera::aAction_PanCamera(Vec2 panCameraFrom, Vec2 panCameraTo, Camera* camera, float durationOfCameraPan, bool lastCameraMovement) {
	mStart = panCameraFrom;
	//this->mOriginalEntity = camera->GetTarget();
	//camera->SetTarget(NULL);
	mCamera = camera;
	mEnd = panCameraTo;
	mTimeSoFar = 0.0f;
	mLastCameraMovement = lastCameraMovement;
	mCamera->mPauseCamera = true;
	//duration is how long
	mDuration = max(durationOfCameraPan, 0.001f);//prevent divide by zero later and to cap durations
}
void aAction_PanCamera::Update(float elapsedTime) {

	mTimeSoFar += elapsedTime;
	float t = mTimeSoFar / mDuration;
	if (t > 1.0f) t = 1.0f;

	mCamera->LookAt((mEnd - mStart) * t + mStart);

	//start with one
	//mCamera->SetTarget(mOriginalEntity);
	//start with another


	if (mTimeSoFar >= mDuration) {
		isDone = true;
		if (mLastCameraMovement) {
			mCamera->mPauseCamera = false;
		}
	}
}


/*

	 x: x location to start the text
	 y: y location to start the text
	 width: width of the window of text
	 height: height of the window of text
	 texDialogueBox: SDL texture of the background image of the text box
	 texCharacterTexture: The list of texture pointers, retrieved from te resouce manager
	 renderer: SDL renderer object
	 writtenDialogue: a script, which is basically a string with the text to write
	 textDelayInMilliseconds:
	 e:
	 delayToProgressDialogueMilliseconds:
	 forceSkipDialogueProgression:


	 Description:
	 Special characters can be input into this script to perform certain actions like skipping a line(semicolon)

*/
aAction_Dialogue::aAction_Dialogue(
	float x,
	float y,
	float width,
	float height,
	SDL_Texture* texDialogueBox,
	SDL_Texture* texCharacterTexture,
	unordered_map<int, SDL_Rect*>* listOfRects,
	SDL_Renderer* renderer,
	const string writtenDialogue,
	unsigned short int textDelayInMilliseconds,
	SDL_Event e,
	unsigned short int delayToProgressDialogueMilliseconds,
	bool forceSkipDialogueProgression
)
{
	mE = e;
	mTextDelayForNextCharacter = new aAction_Delay(textDelayInMilliseconds);
	mDelayToProgressDialogue = new aAction_Delay(delayToProgressDialogueMilliseconds);

	mTextComplete = false;
	mRenderer = renderer;
	mTextBox = texDialogueBox;
	mDialogue = texCharacterTexture;
	mWrittenDialogue = writtenDialogue;
	mMappingToCharacterTexture = listOfRects;

	mTextBoxWidth = width;
	mTextBoxHeight = height;
	mDestRectForTextBox.w = mTextBoxWidth;
	mDestRectForTextBox.h = mTextBoxHeight;

	mDestRectForTextBox.x = mXPos;
	mDestRectForTextBox.y = mYPos;
	mXPos = x;
	mYPos = y;

	mXOffsetFromTextBox = 36;
	mNextXPosIncrement = 38;//this number should be the width of a letter texture
	mCurrentDialogueCharacter = 0;
	mCurrentDialogueWord = 0;
	mStartingDialogueCharacter = 0;

	mLineNumber = 1;
	mAtEndOfTextBox = false;
	mForceSkipLine = false;

	mNextStartingDialogueCharacter = 0;
	mFlashingCounter = 0;
	isDone = false;
	mCheckForProgressDelay = false;
	//split(mWrittenDialogue, mWordTokens, ' ');
	mForceSkipDialogueProgression = forceSkipDialogueProgression;
}

void aAction_Dialogue::Update(float elapsedTime) {

	// which option out of the following two makes the most sense?
	//  1. Initialize ALL letters as a texture, and just point to the ones we want :O
				//if we go with option 1 we need to figure out how to deal with duplicate letters. we will have 2 ppl pointing t thte same texture? or can we render it twice?
				//test that and if we can do this its the best. we can save texture render count
	//  2. Initialize as we go.
	// Display the text box behind the dialogue text:
	// SDL_RenderCopy will return 0 on success, or -1 on error 
	SDL_RenderCopy(mRenderer, mTextBox, NULL, &mDestRectForTextBox);

	//offset of the dialogue text inside the dialogue text box: 
	float dialogueY = mYPos + 46;
	// dialogueX is the starting point of a new line:
	//for (char& c : mWrittenDialogue) {
	//add ability to hit "A" button to move to next section of text. then the text will "slide" up
		//add text box animation to open and close
		//add a new "Spell Bar" texture to appear when yea
		//add the ability to force a new line
		//add max text in a text box	
	/*
		SDL_Rect textRect;
		textRect.w = letterWidth;
		textRect.h = letterHeight;
		*/
		//this maps the ascii value 
						//by casting the char to int, it gives the ascii value. these ascii values line up 
						//to the locations of the characters in the texture when you multiply the ascii value by the width.	
	mDialogueX = mXOffsetFromTextBox;
	int maxStoppingXPointFordialogue = mDestRectForTextBox.w - 100;
	// So this for loop is just 0 to mCurrentDialogueCharacter because we have to re-draw a certain set of characters, 
	// due to the way we delay how long before we draw the next character. if that delay isnt passed, we need to re-draw
	// we need to change this from 0 to current dialogue, to 0 to current word

	//so now mStartingDialogueCharacter should be 
	//can we still leave this as is? can we loop through each word, then each cahr

	//loop through each word and print the character of each
	//through each word..

	//i just need to feed the four loop the character to draw to.

	//so how can i do that with a list of words. and the logic needs to add a space in between words unless its
	//at the end of a line(but it can as long as it doesnt go off the end of the beginning of a new line.... 
	//

	//for each word

	//0 to 

	////right nowe we only incrase mCurrentDialogueCharacter if the delay is up.
	// 
	//lets just do some calculations


	//this method is so poorly designed. and
	//it shows what happens when you dont fully understand and break down all the requirements BEFORE hand. i was to high level....
	//i didnt realize that we needed to write a new word on a new line, and page, and handle special characters. and how exactly to page throgh.
	//re-write this ENTIRE fking method if we end up having processing slowdowns later on dev.+

	//start at word 0. then write all characters as normal. 
	//for current


	//loop through every word
	//see if the next word is after the length or if force skip start a new line, 
	// if we are at the start of a new line, dont print a spacee character
	// if we are at the length of the current word, reset
	// we dont need a current dialogue character.

	for (int x = mStartingDialogueCharacter; x <= mCurrentDialogueCharacter; x++) {

		//Get the first word
		//find length of first word
		//if its past width, start new line
		//if its not, draw it. so i can draw as is, line by line but instead i can see if instead th

		//mWordTokens


		int v = (int)mWrittenDialogue[x];
		// Add checks for "special" characters 
		//FOR EXAMPLE:
		//   1.  if ; character, then force a key press to advance
		//   2. if / character, then decrease the delay time.

		//semicolon:
		if (v == 59) {
			mForceSkipLine = true;
		}

		const auto i = mMappingToCharacterTexture->find(v);

		// Split1
		//  i need to split the string into words
		// then i can count the number 
		// of letters of each word. then see if it will 
		// be written across the edge of the textbox. and if so. 
		// go to the next line!!!

		if (i != mMappingToCharacterTexture->end() || mForceSkipLine)
		{
			if (!mForceSkipLine) {
				// 32 is a space. TODO: create ascii mapping constants file?
				if (!(mDialogueX == mXOffsetFromTextBox && v == 32)) {
					SDL_Rect destRect;
					destRect.w = i->second->w;  //Now we just need to pre-set the width of the smaller characters -> but we should offset this so each char is a width
					destRect.h = i->second->h;  // ALSO NEED TO INCREASE THE OFFSET OF THE NEXT LETTER!.(why?)
					destRect.x = mDialogueX;
					destRect.y = dialogueY;
					// so right now im going through each letter and typing it. i still want to do that. but i want to determine
					// the next line if the current word will overflow the width. 
					// what i need:
					// width of the text box - have this
					// a way to tell length of each word 
					// a word is a grouping of text split by a space.
					// AH! i can split it into an array with split on space
					// then, i can 
					// mapping between the length of each word and the width of text box
					// a place to put this logic


					// isColonCharacter
					// SDL_RenderCopy will return 0 on success, or -1 on error
					SDL_RenderCopy(mRenderer, mDialogue, i->second, &destRect);
					//add to the global texture render list.

				}
				mDialogueX += mNextXPosIncrement;
			}
			// Set boundaries to make sure text does not go outside of the width/height of the textbox:
			if (mDialogueX > (maxStoppingXPointFordialogue) || mForceSkipLine) {

				dialogueY += 56;//56 is the text height with some padding so there is space between lines. TODO: NEED TO MAKE THIS MEMBER VARIABLE
				mDialogueX = mXOffsetFromTextBox;
				mLineNumber += 1;

				if (mLineNumber % 4 == 0) {
					mAtEndOfTextBox = true;
					mNextStartingDialogueCharacter = x + 1;
				}
				mForceSkipLine = false;
			}
		}
	}
	//we draw the text above, so set line number to 0 for the next Action call to this Update function.
	mLineNumber = 0;
	if (!mTextDelayForNextCharacter->isDone) {
		//recursion hehe
		mTextDelayForNextCharacter->Update(elapsedTime);
	}
	else {

		//this is the check for the end of the dialogue. however. we wil need to isntead check for the next "section",	
		// which is AFTER 4 lines have been drawn. if this occurs then we have a full text box, and i want to:

		//1. first we need a linenumber that will increment when a new line is created

		//2. then we need to check if its divisible by 4 OR, if we are at the end of the written dialogue:
				//****************************************************************************************** 
// if divisible by 4 and not end of dialogue, we are DONE when the user hits enter

// if we are divisible by 4 and not end of dialogue, 

// if we are not dibislbe by 4 and not end of idalogue, keep going

// if we are not dibible by 4 any at end of dialogue, poll for user input and done when enter
 //****************************************************************************************** 

//3. set the next start/end loop accordinly based on the next section of text

//if the next character is past the dialogue text passed into the action, poll for "is done" button
		if (mCurrentDialogueCharacter + 1 > mWrittenDialogue.length()) {
			//We are now at the end of the provided dialogue
			//make the polling logic a reusuable method:
			//The below buttons are valid "progression" confirmation buttons

			SDL_PollEvent(&mE);

			if (mE.type == SDL_KEYDOWN) {
				if (mE.key.keysym.scancode == SDL_SCANCODE_RETURN) {
					mCheckForProgressDelay = true;
					//e.key.keysym.scancode = NULL;
					SDL_FlushEvent(mE.type);
				}
			}
			else if (mForceSkipDialogueProgression) {
				mCheckForProgressDelay = true;
			}
		}

		else {
			mCurrentDialogueCharacter += 1;
			mTextDelayForNextCharacter->Restart();
		}

	}

	if (mCheckForProgressDelay) {
		if (!mDelayToProgressDialogue->isDone) {
			mDelayToProgressDialogue->Update(elapsedTime);
		}
		else {
			isDone = true;
		}

	}


}

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}

//
//aAction_BigBang::aAction_BigBang(Entity* object, Entity* target, SDL_Texture* fireball, Animation* explosion, SDL_Renderer* renderer, Camera* cam) {
aAction_BigBang::aAction_BigBang(Entity* object, Vec2 target, SDL_Texture* fireball, SDL_Texture* fireball2, Animation* chant, Animation* cast, SDL_Renderer* renderer, Camera* cam, Sound* sound) {

	mPhase1 = true;
	mPhase2 = false;
	mPhase3 = false;

	//in the future i will make it so once the attack hits, the target will need to run its hit animation. or just "is hit" state and
	//reduced hp which will automatically make its animation run in the main update of an entity. 
	mPlayer = object;
	mTarget = target;

	mFireball = fireball;
	mFireball2 = fireball2;
	//mExplosion = explosion;
	mCast = cast;
	mChant = chant;

	mRenderer = renderer;
	mCamera = cam;

	mSound = sound;

}
void aAction_BigBang::Start() {
	//start fireball above the casters head. 
	mFireballRect.x = mCamera->WorldToScreenX(mPlayer->Center().x);
	mFireballRect.y = mCamera->WorldToScreenY(mPlayer->Center().y) - 250;
	mFireballRect.w = 20;
	mFireballRect.h = 20;

	mScreenExplosion.x = 0;
	mScreenExplosion.y = mCamera->ViewHeight();
	mScreenExplosion.w = mCamera->ViewWidth();
	mScreenExplosion.h = 0;

	mPlayer->mInCutscene = true;
	//mPlayer->mCurrentAnimation = mExplosion;
	mPlayer->setAnimation(mChant->GetCurrentTexture(), 2, 0.4, true);
}

//!!!!!   TODO   !!!!!!!!!!!!!!!!
//add portraits to the dialogue!!
//add custcenein first area "HALT!" dangerous to go ahead!

//step on a button an door opens to allow you to fight a monster
void aAction_BigBang::Update(float fElapsedTime) {

	if (mPhase1) {//phase 1 = growing fireball & spell chant
		int fireballMaxSize = 320;
		if (mFireballRect.w <= fireballMaxSize) {//keep growing until max size

			mPlayer->mCurrentAnimation->AddTime(fElapsedTime);//chant animation progress
			mFireballRect.x -= 2;
			mFireballRect.y -= 2;
			mFireballRect.w += 4;
			mFireballRect.h += 4;
		}
		else {
			mPhase1 = false;
			mPhase2 = true;
			mPlayer->setAnimation(mCast->GetCurrentTexture(), 1, 0.4, true);//set animation to cast
		}

		SDL_RenderCopy(mRenderer, mFireball, NULL, &mFireballRect);

	}
	else if (mPhase2) {//phase 2 = throwing fireball and spell cast
		Vec2 fireballPosition;
		if (mCurrentLerp == nullptr) {//lerp the ball to the target
			Vec2 start(mFireballRect.x, mFireballRect.y);
			fireballPosition = Vec2(mFireballRect.x, mFireballRect.y);
			mCurrentLerp = new Lerp(start, mTarget, fireballPosition, 1);
		}
		else {
			if (mCurrentLerp->update(fElapsedTime)) {
				//target reached
				mPhase2 = false;
				mPhase3 = true;

				mSound->PlaySFX(SoundConstants::S_WAV_INFERNO, -1, 1);
				// Move the explosion down w
				mFireball2Rect.x = mFireballRect.x;
				mFireball2Rect.y = mFireballRect.y + 120;
				mFireball2Rect.w = mFireballRect.w;
				mFireball2Rect.h = mFireballRect.h;
			}
		}
		mFireballRect.x = mCurrentLerp->mPosition.x;
		mFireballRect.y = mCurrentLerp->mPosition.y;
		SDL_RenderCopy(mRenderer, mFireball, NULL, &mFireballRect);

	}
	else if (mPhase3) {//phase 3 = exploding fireball 
		// first stage explosion begin!

		mFireball2Rect.x -= 8;
		mFireball2Rect.y -= 8;
		mFireball2Rect.w += 16;
		mFireball2Rect.h += 16;

		int fireballMaxSize = 1000;
		if (mFireball2Rect.w <= fireballMaxSize) {

			SDL_RenderCopy(mRenderer, mFireball2, NULL, &mFireball2Rect);
		}
		else {
			mPlayer->mInCutscene = false;
			mPhase3 = false;
			mPhase4 = true;
			SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
		}
		//transform to new explosion and grow to full screen in about 5 frames. then less opaque. more white. then explosions
	}
	else if (mPhase4) {// Wall of light and explosions
		if (mScreenExplosion.y < -3500) {
			mPhase4 = false;
			// Cleanup !!
			isDone = true;
			mPlayer->mInCutscene = false;
		}
		else {
			mScreenExplosion.y -= 60;
			mScreenExplosion.h += 60;

			mPlayer->mInCutscene = true;
			//set explosion animations randomly on the screen a random area for the explosion
		}

		//the code below is lerping between the start and end RGB and A values to slowly transition between the two
		SDL_SetRenderDrawColor(mRenderer, 250, 250, 250, 230);
		SDL_RenderFillRect(mRenderer, &mScreenExplosion);

	}
	//phase5 
	//explosions and play sound!!


}
// 
//bool aAction_BigBang::Lerp::Update(float timesofar) {
//	{
//		mTimeSoFar += timesofar;
//		float t = mTimeSoFar / mDuration;
//		if (t > 1.0f) t = 1.0f;
//
//		mPosition.x = ((mEnd.x - mStart.x) * t + mStart.x);
//		mPosition.y = ((mEnd.y - mStart.y) * t + mStart.y);
//
//		//mMoveableObject->mVelocity.x = (mEnd.x - mStart.x) / mDuration;
//		//mMoveableObject->mVelocity.y = (mEnd.y - mStart.y) / mDuration;
//
//		if (mTimeSoFar >= mDuration) {
//			mPosition.x = mEnd.x;
//			mPosition.y = mEnd.y;
//
//			return true;
//			//if (mEndCutscene) mMoveableObject->mInCutscene = false;
//		}
//		else {
//			return false;
//		}
//	}
//}