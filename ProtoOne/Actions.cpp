#include "Actions.h"
#include "Player.h"

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
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
aAction_MoveTo::aAction_MoveTo(Entity* object, Vec2 end, float duration) {
	mMoveableObject = object;
	mEnd = end;

	mTimeSoFar = 0.0f;
	mDuration = max(duration, 0.001f);//prevent divide by zero later and to cap durations
	mVel = 2.0f;
	//mDir = end - mStart;
	//mDir.Normalize();
	//mDir *= 1; 
}

void aAction_MoveTo::Start() {

	mStart = mMoveableObject->Center();
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
	mMoveableObject->addTimeToAnimation(dt);
	//mMoveableObject->SetCenter(mMoveableObject->Center() + mDir * mVel);
	if (mTimeSoFar >= mDuration) {
		mMoveableObject->SetCenter(mEnd);
		mMoveableObject->mVelocity.x = 0;
		mMoveableObject->mVelocity.y = 0;
		isDone = true;
	}


}

//the higher the duration, the slower fade
//TODO: add option to fade in our out depending on flag!!!!
aAction_FadeIn::aAction_FadeIn(int w, int h, float duration, SDL_Renderer* renderer, int r, int g, int b) {
	mW = w;
	mH = h;

	mR = r;
	mG = g;
	mB = b;

	mDecrementer = 1000 / duration;
	mRenderer = renderer;
}

/*
!!!!!!!!!!!!!!!!!!!!!!!!
!!!FADE IN FROM BLACK!!!
!!!!!!!!!!!!!!!!!!!!!!!!
*/
//TODO: add a direction to start in so we can face the player that way.
void aAction_FadeIn::Start() {
	//start out with no opacity at all; the rectangle color is completely solid 
	mOpacity = 255;
	mFadeRect.x = 0;
	mFadeRect.y = 0;
	mFadeRect.w = mW;
	mFadeRect.h = mH;
	//isStarted = true;//I THINK THIS IS DONE IN PROCESS ACTION CODE AFTER START. CONFIRM THIS AND IF SO REMOVE LINE
}
void aAction_FadeIn::Update(float fElapsedTime) {
	//its basically a black rectange overlaying the screen that gets increasingly opaque  

	//SDL_Render idea:
		//time passed divided by duration to get actual increment value??? verify	
		//incrementer is duration in seconds we want the process to take. we divide by elapsed time because 
	//NEED TO CREATE LAYERS, so we can have the flashes and keep character sprites visible

	if (mOpacity <= 0) {
		isDone = true;
	}
	//color dictionary:
		//white: 255,255,255  (lightning)
		//black: 0,0,0
		//other: 0,0,0
		//green: 0,0,0
		//orange: 0,0,0
		//Red: 0,0,0

	//i need to convert the position in the texture, to the
	SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(mRenderer, mR, mG, mB, mOpacity);

	mOpacity = mOpacity -= mDecrementer;
	if (mOpacity < 0) { mOpacity = 0; }
	//we want it to fade is slow AT FIRST then speed up at the end. so increment exponentially. need another variable or some maths here.
	//also need to have parameter specify for how quick to increment
	mDecrementer = mDecrementer * 1.02;

	SDL_RenderFillRect(mRenderer, &mFadeRect);
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
Dialogue documentation:
	-Add an action and specifiy:
		x and y location to start the text
		width and height of the window of text
		the background image of the text box
		need the list of texture pointers, retrieved from te resouce manager
		SDL renderer object
		a script, which is basically a string with the text to write
			special characters can be input into this script to perform certain actions like skipping a line(semicolon)

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
	unsigned short int delayToProgressDialogueMilliseconds,
	bool forceSkipDialogueProgression
)
{
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
			SDL_Event e;
			SDL_PollEvent(&e);

			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
					mCheckForProgressDelay = true;
					//e.key.keysym.scancode = NULL;
					SDL_FlushEvent(e.type);
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
//cout << "character: " << c << " in ascii value is: " << (int)c << " and divisible by 65 it is: " << v  << endl;

//	bool isColonCharacter = (v == 58);
//	bool isSpaceCharacter = (v == 32);
//	bool isExclamationMark = (v == 33);
//	bool isQuestionMark = (v == 63);
//	bool isDoubleApostrophe = (v == 34);
//	bool isApostrophe = (v == 39);
//	bool isPeriod = (v == 46);
//	//can also determine the nextXPosition based on above mapping

//	int nextXPosIncrement = 38;
//	if (isSpaceCharacter) {
//*		textRect.y = 0;
//		textRect.x = letterWidth * 12;*/
//	}
//	else if (isColonCharacter) {
//		//textRect.y = letterHeight * 3;
//		//textRect.x = letterWidth;

//		//INSTEAD OF LOOKING UP THESE VALUES, CHANGE THE INITIAL CREATION HEIGHT/WIDTH TO COMPENSATE SO ALL WE NEED TO DO HERE IS TAKE THE RECTS
//		nextXPosIncrement = 32;
//		dialogueY += 2;
//	}
//	else if (isApostrophe) {
//		textRect.y = letterHeight * 3;
//		textRect.x = letterWidth * 3;
//		nextXPosIncrement = 32;
//	}
//	else if (isExclamationMark) {
//		textRect.y = 0;
//		textRect.x = letterWidth * 10;
//		nextXPosIncrement = 21;
//	}
//	else if (isQuestionMark) {
//		textRect.y = 0;
//		textRect.x = letterWidth * 11;
//		nextXPosIncrement = 21;
//	}
//	else if (v >= 48 && v <= 57) {
//		//NUMBERS
//		v = v % 48;
//		textRect.y = 0;
//		textRect.x = v * letterWidth;
//	} 
//	else if (v >= 65 && v <= 90) {
//		//UPPERCASE LETTERS
//		v = v % 65;
//		textRect.y = letterHeight;
//		textRect.x = v * letterWidth;
//	}
//	else if (v  >= 97 && v  <= 122) {
//		//LOWERCASE LETTERS
//		v = v % 97;
//		textRect.y = letterHeight * 2;
//		textRect.x = v * letterWidth;
//	}



template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}

//LERP
	//Vec2 dirToEnd = mEnd - mStart;
	//dirToEnd.Normalize();
	//float speedMultiplyer = 3.0;		 
	//float degreeofDirection = 0.0f;		
	//degreeofDirection = Rad2Deg(atan2( dirToEnd.y, dirToEnd.x ));
	// When creating the action, we specify the direction we want the sprite to face. i mean we might want him to moonwalk :)
	//Vec2 nextPosition = mMoveableObject->Center() + (dirToEnd * speedMultiplyer);		
	//mStart = nextPosition;

//mPlayer->SetCenter(mPlayer->Center() + 20 * mPlayer->mMoveSpeedScale * dt * moveVec);		
//need normalized vector of the direction to the end
//if objects location @ end location, were done
// move along the forward/backward axis