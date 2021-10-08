#include "Gameplay.h"
#include "Game.h"
#include "Util.h"
#include "GlobalsAreBad.h" 
#include "ResourceManager.h"
#include "SoundConstants.h"
#include "StoryScriptConstants.h"
#include "BackgroundConstants.h"
#include "Actions.h"
#include <iostream>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;


Gameplay::Gameplay(Game* game, Sound* sound)
	: GameState(game, sound)
	, mPlayer(NULL)
	, mMissiles()
	, mPlayerTex(NULL)
	, mShotTex(NULL)
	, mWorldWidth(0)
	, mWorldHeight(0)
	, mIsActive(false)
	, mCamera(NULL)
	, mGameplayKeyboardHandler()
	, mDialogueMode(false)
	, mGameRenderer(mGame->GetRenderer())

{
}

Gameplay::~Gameplay()
{
}

void Gameplay::Initialize()
{
	SDL_Renderer* renderer = mGame->GetRenderer();

	//***********************************
	//*********   Main Player   *********
	//***********************************
	mShotTex = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_FIREBALL, renderer);

	////*********************************
	////*********   NPCs   **************
	////*********************************
	//mEnemyTex = ResourceManager::Acquire("media/mysteriousGem.png", renderer);
	//mEnemyTex = LoadTexture("media/meteor.png", renderer);	
	mEnemyTex = LoadTexture("media/dragons/bahamut.gif", renderer);
	//mEnemyTex = LoadTexture("media/Spaceship-Drakir3.png", renderer);

	//****************************************
	//*********   Initial Background  ********
	//****************************************
	mBgTexBack = ResourceManager::Acquire("media/background/SnowCliff_Back.png", renderer);
	mBgTexFront = ResourceManager::Acquire("media/background/SnowCliff_Front.png", renderer);

	mNarsheBackground = ResourceManager::Acquire("media/background/SnowCliff_Back.png", renderer);
	mNarsheForeground = ResourceManager::Acquire("media/background/SnowCliff_Front.png", renderer);
	mDesertBackground = ResourceManager::Acquire("media/background/desert/desertX4.png", renderer);
	//mCurrentBackground = ResourceManager::Acquire("media/background/desert/figaroCastleX4.png", renderer);

	//mBgTex = ResourceManager::Acquire("media/background/darkforest.png", renderer);
	//mBgTex = LoadTexture("media/background/blah.jpg", renderer);
	//mTextImage = ResourceManager::Acquire("media/DialogueText.png", renderer);
	mTextImage = ResourceManager::Acquire("media/DialogueText_32x32.png", renderer);
	//mTextBoxFF6 = ResourceManager::Acquire("media/textBox.png", renderer);	
	mTextBoxFF6 = ResourceManager::Acquire("media/textBox_1080.png", renderer);
	/*mBgTex = LoadTexture("media/background/fieryBackground.png", renderer);*/
	//mExplosionTex = LoadTexture("media/explosion.tga", renderer);

}

void Gameplay::Shutdown()
{
	ClearLevel();

	SDL_DestroyTexture(mPlayerTex);
	SDL_DestroyTexture(mShotTex);
	SDL_DestroyTexture(mNarsheForeground);
	SDL_DestroyTexture(mNarsheBackground);
	SDL_DestroyTexture(mEnemyTex);
	SDL_DestroyTexture(mExplosionTex);
	SDL_DestroyTexture(mTextBoxFF6);
	SDL_DestroyTexture(mDesertBackground);
	SDL_DestroyTexture(mBgTexBack);
	SDL_DestroyTexture(mExplosionTex);
	SDL_DestroyTexture(mTextImage);
	SDL_DestroyTexture(Text);


}

void Gameplay::OnWindowResized(int w, int h) {
	mCamera = new Camera((float)mGame->GetScreenWidth(),
		(float)mGame->GetScreenHeight(),
		(float)mWorldWidth,
		(float)mWorldHeight);

	std::cout << "Screen width in gameplay window resize: " << (float)mGame->GetScreenWidth() << std::endl;
	std::cout << "Screen height in gameplay window resize: " << (float)mGame->GetScreenHeight() << std::endl;

}

void Gameplay::LoadLevel()
{

	SDL_Renderer* renderer = mGame->GetRenderer();
	//ClearLevel();
	//whatever the size of the background is the size of the world....
	//SDL_QueryTexture(ResourceManager::Acquire(BackgroundConstants::FIGARO_ANIMATION, renderer), NULL, NULL, &mWorldWidth, &mWorldHeight);

	//mWorldWidth = 4096;
	//mWorldHeight = 3465;

	//std::cout << "World size is " << mWorldWidth  << "x" << mWorldHeight << std::endl;

	//mCurrentBackground = new Animation(ResourceManager::Acquire(BackgroundConstants::FIGARO_ANIMATION, renderer), 3, 0.2, true, SDL_FLIP_NONE, true);
	mCurrentBackground = new Animation(ResourceManager::Acquire(BackgroundConstants::NARSHE_FRONT, renderer), 1, 0.2, true, SDL_FLIP_NONE, true);

	//set world size based on background:
	SDL_QueryTexture(mCurrentBackground->GetCurrentTexture(), NULL, NULL, &mWorldWidth, &mWorldHeight);

	mDistantBackground = new Animation(ResourceManager::Acquire(BackgroundConstants::NARSHE_BACK, renderer), 1, 0.2, true, SDL_FLIP_NONE, true);


	//mforeground = new Animation(ResourceManager::Acquire(BackgroundConstants::NARSHE_BACK, renderer), 1, 0.2, true, SDL_FLIP_NONE, true);
	//for the intro scene, we want to pan the camera through multiple screens. and have characters moving so here is the following actions:
	//summon giga gaia.
	//other abilities
	//then they get stolen ;)
	/*

	1. FADE IN FROM BLACK
	2. MUSIC BEGINS
	3. screen pans left and down across an area talking about the demon war. spells fire.
	4. screen darkens
	5. LEVEL CHANGES TO A DIFFERENT BACKGROUND
	6. Fade in
	7. peace at last thanks to the efforts of the brave specialists and team of good
	8. but now.....
	7. train noise...
	8. fade in
	9. screen

	*/

	//*********************************************************************
	// initialize the player
	//********************************************************************* 
	/*
		tips:
		OFF THE LEFT SIDE OF THE SCREEN:
			mPlayer->SetCenter(-50.0f, 0.5f * mWorldHeight);

		MIDDLE OF THE SCREEN:
			mPlayer->SetCenter(0.5f * mWorldWidth, 0.5f * mWorldHeight);

		//mPlayer->SetOrientation(DIR_DOWN);
		//mPlayer->SetSpriteAngleCorrection(DIR_UP);
	*/

	//use this to zoom. but everything grows. so might not be the best option.
	//probably just a fade or something.... but can try a cool zoom type option for bosses
	SDL_RenderSetScale(renderer, 1, 1);

	/*

		NPCs
		Event indicators / cutscene starters
		Enemies
		Treasure chests
		Signs
		Save Points

	*/

	//****************
	//LOAD MAP EFFECTS
	//****************

	//UPDATE EFFECTS TO ADD OPAQUE RENDER. 
	// OR AT LEAST CREATE A SPELL ACTION THAT CHANGES THE OPAQUENESS of textures. 
	//it will take create an effect that doesnt loop
		//texture
		//position start
		//position end.

	//MAKE CHANGING AREAS
	//MAKE AN INTRO LIKE OCTOPATH

	Game* game = GetGame();

	//*//**************//
	//*/////////////////*
	//*               //*
	//* Camera Init   //*
	//*               //*
	//*/////////////////*
	//*****************//
	mCamera = new Camera((float)game->GetScreenWidth(),
		(float)game->GetScreenHeight(),
		(float)mWorldWidth,
		(float)mWorldHeight);

	//*******************************************************************************
	//do NOT set arbitrary values for the player position
	//always set players within the world! or else issues can happen with the camera!
	//*******************************************************************************

	//set the player in the world, then set the camera on the player
	mPlayer = new Player(ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_UP, mGameRenderer), 4, 1, true,40,50);

	mPlayerTex = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_WALK_UP, renderer);
	mPlayer->mWalkDownTexture = ResourceManager::Acquire(PlayerConstants::TIME_LORD + PlayerConstants::WALK_DOWN, renderer);
	mPlayer->mWalkUpTexture = ResourceManager::Acquire(PlayerConstants::TIME_LORD + PlayerConstants::WALK_UP, renderer);
	mPlayer->mWalkLeftTexture = ResourceManager::Acquire(PlayerConstants::TIME_LORD + PlayerConstants::WALK_LEFT, renderer);
	mPlayer->mWalkRightTexture = ResourceManager::Acquire(PlayerConstants::TIME_LORD + PlayerConstants::WALK_LEFT, renderer); //this is just the left animation flipped. and its flipped in player update method
	mPlayer->mRunDownTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_DOWN, renderer);
	mPlayer->mRunUpTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_UP, renderer);
	mPlayer->mRunLeftTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_LEFT, renderer);
	mPlayer->mRunRightTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_RIGHT, renderer);


	mPlayerTex = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_WALK_UP, renderer);

	//mPlayer->SetCenter(mCamera->WorldToScreen(500, 500).x, mCamera->WorldToScreen(500, 500).y);//minus player height

	Vec2 playerStartingPos(mWorldWidth - 700, mWorldHeight - mPlayer->Top());

	//intro is walking up narsh cliff
	mPlayer->SetCenter(playerStartingPos.x, playerStartingPos.y);//minus player height

	std::cout << "player x and y: " << mPlayer->Center().x << " " << mPlayer->Center().y << " " << std::endl;

	std::cout << "hitbox info: " << mPlayer->HitBoxBottom() << " "<< mPlayer->HitBoxLeft() <<  " " << mPlayer->HitBoxRight() << " " << mPlayer->HitBoxTop() << " "<< std::endl;


		/*
		LOAD GIGA GAIA
		*/
		//Enemy* e = new Enemy(ResourceManager::Acquire(EnemyConstants::GIGA_GAIA_IDLE, renderer));
		//float x = mCamera->ViewLeft();
		//float y = mCamera->ViewBottom();
		//e->SetCenter(x + 100, y + 300);
		//e->SetLayer(1);
		//e->SetState(ENEMY_HOVER);
		//e->SetSpeedScale(0);

	for (int i = 0; i < 50; i++) {
		Enemy* greenDragon = new Enemy(ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer), 3, 0.5, true);
		greenDragon->SetCenter(rand() % mWorldWidth, rand() % mWorldHeight);
		greenDragon->SetLayer(1.0f);
		greenDragon->SetState(ENEMY_PATROL);
		//greenDragon->SetSpeedScale(1);

		greenDragon->mRunUpTexture = ResourceManager::Acquire("./media/dragonFlyUpX4.png", renderer);
		greenDragon->mRunLeftTexture = ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer);
		greenDragon->mRunDownTexture = ResourceManager::Acquire("./media/dragonFlyDownX4.png", renderer);
		greenDragon->mRunRightTexture = ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer);
		mEnemies.push_back(greenDragon);
	}



	Vec2 guardOneStartingPos(playerStartingPos.x -50, playerStartingPos.y - 500);
	Vec2 guardTwoStartingPos(playerStartingPos.x + 50, playerStartingPos.y - 500);
	
	NPC* guardOne = new NPC(ResourceManager::Acquire("./media/characters/zozma/walkdown.png", renderer), 4, 1.2f, true, 50, 50);
	guardOne->SetCenter(guardOneStartingPos);
	NPC* guardTwo = new NPC(ResourceManager::Acquire("./media/characters/zozma/walkdown.png", renderer), 4, 1.2f, true, 50, 50);
	guardTwo->SetCenter(guardTwoStartingPos);

	 
	guardOne->mName = "Guard One";
	guardTwo->mName = "Guard Two";
	
	guardOne->SetCenter(guardOneStartingPos.x, guardOneStartingPos.y);
	guardTwo->SetCenter(guardTwoStartingPos.x, guardTwoStartingPos.y);

	guardOne->mDialogue = "Hi! I'm Guard One. Pleasure is mine!";
	guardTwo->mDialogue = "Guard two reporting for duty, SIR!!";

	mNPCs.push_back(guardOne);
	mNPCs.push_back(guardTwo);

	//draw him in the sky. maybe make an action to make something visible or unvisible.
	Animation* sparkle = new Animation(ResourceManager::Acquire("./media/zozma/walkdown.png", renderer), 4, 3, true);

	//Enemy* greenDragon = new Enemy(ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer), 3, 0.5, true);
	//greenDragon->SetCenter(500, 500);
	//greenDragon->SetLayer(1);
	//greenDragon->SetState(ENEMY_PATROL);
	////greenDragon->SetSpeedScale(1);

	//greenDragon->mRunUpTexture = ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer);
	//greenDragon->mRunLeftTexture = ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer);
	//greenDragon->mRunDownTexture = ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer);
	//greenDragon->mRunRightTexture = ResourceManager::Acquire("./media/dragonFlyLeft.png", renderer);
	//greenDragon->setAnimationTexture(greenDragon->mRunDownTexture);

	/*
	Load Dragon off screen
	*/
	//Enemy* ninjaBlue = new Enemy(ResourceManager::Acquire(EnemyConstants::NINJA_BLUE, renderer));
	//ninjaBlue->SetCenter(500, 500);
	//ninjaBlue->SetLayer(1);
	//ninjaBlue->SetState(ENEMY_HOVER);
	//ninjaBlue->SetSpeedScale(0);

	Enemy* ninjaBlue2 = new Enemy(ResourceManager::Acquire(EnemyConstants::NINJA_BLUE, renderer));
	ninjaBlue2->SetCenter(500, 700);
	ninjaBlue2->SetLayer(1.0f);
	ninjaBlue2->SetState(ENEMY_HOVER);
	ninjaBlue2->SetSpeedScale(1);
	/*
		mEnemies.push_back(e);*/

		/*
		mEnemies.push_back(ninjaBlue);
		mEnemies.push_back(ninjaBlue2);*/

		//
		// create some enemies
		//
		/*for (int i = 0; i < 5; i++) {
			Enemy* e = new Enemy(mEnemyTex);
			e->SetSpriteAngleCorrection(DIR_DOWN);
			float x = RandomFloat(e->Radius(), mWorldWidth - e->Radius());
			float y = RandomFloat(e->Radius(), mWorldHeight - e->Radius());
			e->SetCenter(x, y);
			e->SetAngle(RandomFloat(0.0f, 360.0f));
			mEnemies.push_back(e);
		}*/

		//*//**************//
		//*/////////////////*
		//*               //*
		//* INTRO CUTSCENE//*
		//*               //*
		//*/////////////////*
		//*****************//
	ResourceManager::initializeTextBoxTextures(renderer);
	ResourceManager::PopulateCharacterRects();

	int screenHeight = game->GetScreenWidth();
	int screenWidth = game->GetScreenWidth();

	Animation* nextBackground = new Animation(ResourceManager::Acquire(BackgroundConstants::FIGARO_ANIMATION, renderer), 3, 0.2, true, SDL_FLIP_NONE, true);

	//mGame->mScriptProcessor.AddAction(new aAction_ChangeBackground(mCurrentBackground, nextBackground));
	//Game->mScriptProcessor.AddAction(new aAction_FadeIn(screenWidth, screenHeight, 3, mGame->GetRenderer(), 0, 0, 0, 0, 0, 0, 255, 255));
	/*
	mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 1200,1900 }, { 1800,1900 }, mCamera, 5.0f));
	mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 1800,1900 }, { 1800,2800 }, mCamera, 5.0f, true));*/

	//pan the camera from right side of the screen to the left

	//walk the character there as well
	//mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ screenWidth,1200 }, { 1200,1200 }, mCamera, 5.0f));

	// 
	//aAction_FadeIn* fadeIn2 = new aAction_FadeIn(screenWidth, screenHeight, 3, mGame->GetRenderer(), 0, 0, 0, 0, 0, 0, 0, 255);
	//aAction_FadeIn* fadeOut2 = new aAction_FadeIn(screenWidth, screenHeight, 3, mGame->GetRenderer(), 0, 0, 0, 0, 0, 0, 255, 0);
	//
	////fade to black then go back to the previous
	//aAction_ChangeBackground* bkgd2 = new aAction_ChangeBackground(mCurrentBackground, nextBackground, &mWorldWidth, &mWorldHeight, mCamera);
	//mGame->mScriptProcessor.AddAction(new aAction_ChangeLevel(fadeOut2, fadeIn2, bkgd2));

	//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, {x, y - 500}, 2.0f, true));
	//x = x - 500;
	bool mShowIntro = false;
	if (mShowIntro) {
		float x = mPlayer->Center().x;
		float y = mPlayer->Center().y - 800;

		//mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ x, y - 600 }, { x, y }, mCamera, 5.0f, true));
		mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { x, y }, 2.0f));

		x = x - 1000;
		mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { x, y }, 2.0f));

		y = y - 680;
		mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { x, y }, 2.0f, true));

		//mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 1200,1200 }, { 1200,1900 }, mCamera, 5.0f));
		//fade out to black and then change scene to figaro, then fade in from black:
		aAction_FadeIn* fadeIn = new aAction_FadeIn(screenWidth, screenHeight, 3, mGame->GetRenderer(), 0, 0, 0, 0, 0, 0, 0, 255);
		aAction_FadeIn* fadeOut = new aAction_FadeIn(screenWidth, screenHeight, 3, mGame->GetRenderer(), 0, 0, 0, 0, 0, 0, 255, 0);
		aAction_ChangeBackground* bkgd = new aAction_ChangeBackground(mCurrentBackground, nextBackground, &mWorldWidth, &mWorldHeight, mCamera);

		aAction_ChangeLevel* chg1 = new aAction_ChangeLevel(fadeOut, fadeIn, bkgd);
		chg1->startNextAction = true;
		mScriptProcessor_CharacterMovements.AddAction(chg1);

		aAction_PanCamera* a = new aAction_PanCamera({ x,y }, { x + 500,y }, mCamera, 5.0f, true);
		//a->startNextAction = true;
		mScriptProcessor_CharacterMovements.AddAction(a);

		//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { x + 600, y }, 2.0f));
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { x, y - 600 }, 3.0f));
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { x - 600, y }, 3.0f, true));
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { x - 600, y }, 3.0f));

		mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 2200,1200 }, { 1200,1200 }, mCamera, 5.0f));
		mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 1200,1200 }, { 1200,1900 }, mCamera, 5.0f));
		mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 1200,1900 }, { 1800,1900 }, mCamera, 5.0f));
		mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 1800,1900 }, { 1800,2800 }, mCamera, 5.0f, true));

		//mScriptProcessor_Effects.AddAction(new aAction_PanCamera({ 1800,1900 }, { 1800,2800 }, mCamera, 5.0f, true));
		//keep the screen blacked out for a period of time:
		//mGame->mScriptProcessor.AddAction(new aAction_FadeIn(width, height,10, mGame->GetRenderer(), 255, 255, 255,255,255,255,255,255));

		int width = mGame->GetScreenWidth();
		int height = mGame->GetScreenHeight();

		mGame->mScriptProcessor.AddAction(new aAction_FadeIn(width, height, 5, mGame->GetRenderer(), 255, 255, 255, 0, 0, 0, 255, 0));
		mGame->mScriptProcessor.AddAction(new aAction_FadeIn(width, height, 5, mGame->GetRenderer(), 0, 0, 0, 255, 255, 255, 0, 255));
		mGame->mScriptProcessor.AddAction(new aAction_FadeIn(width, height, 5, mGame->GetRenderer(), 255, 255, 255, 255, 255, 255, 255, 255));
		//mGame->mScriptProcessor.AddAction(new aAction_Delay(2000)); //will this work on all systems? 
		//time it so by this time, the effects script processor will change the location. or have an action that calls this fade in, then calls the change

		//mTextBoxFF6
		mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, NULL, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE, 5, mGame->mE, 2000, true));
		mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, NULL, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE_4, 5, mGame->mE, 2000, true));

		// End cutscene on the last movement! (bool argument at the end)
		// mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, { mCamera->ViewLeft()+500, 3100 }, 2.0f));


		//std::cout << "World size is " << mWorldWidth  << "x" << mWorldHeight << std::endl;
		//TODO: create constants for the fireball animation. growing yellow ball basically
		//another animation of a semicircle sphere yellow ball
		//Animation* fireball = new Animation(ResourceManager::Acquire(BackgroundConstants::FIGARO_ANIMATION, renderer), 3, 0.2, true, SDL_FLIP_NONE, true);

		//Animation* fireball = new Animation(ResourceManager::Acquire(BackgroundConstants::FIGARO_ANIMATION, renderer), 3, 0.2, true, SDL_FLIP_NONE, true);
		//mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE_3, 5, mGame->mE));

		//SDL_Texture* t[] = {
		//	ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_HIT,renderer),
		//	ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_HURT, renderer)
		//};

		//mScriptProcessor_CharacterMovements.AddAction(new aAction_ChangeAnimation(mPlayer, t, 3000));
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(e, Vec2(e->Center().x, e->Center().y - 500), 1.5f, false));

		//easrthquake, charge, cast spell, flash of lightning. bot3 lightning hits bof character
		//ahem, sorry, he was in the wrong game....i will be the narrator of this plot and it is just
		// a short showcase of what this engine allows :) and provides you with certain....creative liberties... ;)  
		//summon an esper!
		//talk to giga gaia. block an attack from his hand. 
		//have the two characters move down to the same position

		//mScriptProcessor_Effects.AddAction(new aAction_MoveTo(mPlayer2, Vec2(newplayerX - 20, newplayerY), 2.0f));
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, Vec2(newplayerX, newplayerY), 2.0f));
		//mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE_2, 45));
		//mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE_2, 45, mGame->mE));

		//list of entities  with a list of durations and a list of locations 
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::AFTER_FLASH, 45));
		//once they press enter, she should wink and smile

		//mScriptProcessor_Effects.AddAction(new aAction_FadeIn(width, height, 100, mGame->GetRenderer(), 200, 10, 0));
		//mScriptProcessor_Effects.AddAction(new aAction_FadeIn(width, height, 100, mGame->GetRenderer(), 200, 10, 200));
		//mScriptProcessor_Effects.AddAction(new aAction_FadeIn(width, height, 100, mGame->GetRenderer(), 200, 200, 0));

		//then we will say more and walk and introduce another character
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, "This is a prototype RPG Engine that I worked; tirelessly on...; It is almost at a presentable point now!", 45));
		//mScriptProcessor_CharacterMovements.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, "I hope you enjoy it!", 45));

		//ENEMY ACTIONS
		//mGame->mScriptProcessor.AddAction(new aAction_MoveTo(ninjaBlue,Vec2(100,100), 2.5f,false) );

		//mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, Vec2(newplayerX, newplayerY), 2.5f));
		//mGame->mScriptProcessor.AddAction(new aAction_MoveTo(mPlayer, Vec2(0, newplayerY), 6.0f));

		//MUSIC!
		mSound->playMusicFadeIn(SoundConstants::M_MP3_SUNCITY, -1, 1000);

	}//end show intro


	//MUSIC
	mSound->playMusicFadeIn(SoundConstants::M_MP3_SUNCITY, -1, 1000);

	// make camera follow the player
	mCamera->SetTarget(mPlayer);


}

void Gameplay::ClearLevel()
{
	//delete mPlayer;
	//mPlayer = NULL;
	//Clear player textures:
	//TODO: later on move this task to a resourceManager. 
	for (auto it = mMissiles.begin(); it != mMissiles.end(); it++) {
		Missile* m = *it;
		delete m;
	}
	mMissiles.clear();

	for (auto it = mEnemies.begin(); it != mEnemies.end(); it++) {
		delete *it;
	}
	mEnemies.clear();

	delete mCamera;
	mCamera = NULL;
}


void Gameplay::Update(float dt)
{
	//std::cout << "World Width: "<< mWorldWidth << std::endl;
	//std::cout << "World Height: " << mWorldHeight << std::endl;
	const Uint8* keys = mGame->GetKeys();
	//const Uint8* keyState = mGame->GetGameplayState();	

	//mScript.ProcessActions(dt);

	// move along world axes
	// get position of cursor (in screen space)
	//MOUSE TRACKING :) 
	/*
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	//may need to get from a currentState variable instaed of just gameplay in the future if it makes sense
	Camera* camera = game->GetGameplayState()->GetCamera();
	// translate screen space mouse coordinates to world space
	Vec2 mousePos = camera->ScreenToWorld(mouseX, mouseY);
	// compute vector from player to mouse
	Vec2 toMouse = mousePos - mCenterPos;
	// compute angle towards mouse pos
	float angle = std::atan2(toMouse.y, toMouse.x);
	// convert angle to degrees
	mAngle = Rad2Deg(angle);
	*/

	//std::cout << "A state: " << mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_A) << std::endl;
	//std::cout << "SHIFT state: " << mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT) << std::endl;

	//if (mScript->userControlEnabled) {

	if (mScriptProcessor_CharacterMovements.userControlEnabled && mGame->mScriptProcessor.userControlEnabled) {


		/*else {
			mPlayer->SetState(mPlayer->WALKING);
		}*/

		//priority order: u,left,right,down
		if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_W)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running			
			mPlayer->mVelocity.y = 0;
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_W);
		}
		else if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_A)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running
			//we want to swap between current chars with "R". so 						
			//load texture returns a pointer...so what, were gonna have a pointer to a pointer? hmm probably a better way.			
			mPlayer->mVelocity.x = 0;
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_A);
		}
		else if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_S)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running			
			//mPlayer->moveDown(moveVec, dt, false);
			mPlayer->mVelocity.y = 0;
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_S);
		}
		else if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_D)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running			
			//mPlayer->moveRight(moveVec, dt, false);
			mPlayer->mVelocity.x = 0;
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_D);
		}

		if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_W)) {
			mPlayer->mVelocity.y = -2;
			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				mPlayer->SetState(mPlayer->RUNNING);
				mPlayer->mVelocity.y *= PlayerConstants::RUN_SPEED_MODIFYER;
			}
			else {
				mPlayer->SetState(mPlayer->WALKING);
			}
		}
		if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_A)) {
			mPlayer->mVelocity.x = -2;
			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				mPlayer->SetState(mPlayer->RUNNING);
				mPlayer->mVelocity.x *= PlayerConstants::RUN_SPEED_MODIFYER;
			}
			else {
				mPlayer->SetState(mPlayer->WALKING);
			}
		}
		if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_S)) {
			mPlayer->mVelocity.y = 2;
			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				mPlayer->SetState(mPlayer->RUNNING);
				mPlayer->mVelocity.y *= PlayerConstants::RUN_SPEED_MODIFYER;
			}
			else {
				mPlayer->SetState(mPlayer->WALKING);
			}
		}
		if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_D)) {
			mPlayer->mVelocity.x = 2;
			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				mPlayer->SetState(mPlayer->RUNNING);
				mPlayer->mVelocity.x *= PlayerConstants::RUN_SPEED_MODIFYER;
			}
			else {
				mPlayer->SetState(mPlayer->WALKING);
			}
		}

		if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_SPACE)) {
			int width = mGame->GetScreenWidth();
			int height = mGame->GetScreenHeight();
			mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_SPACE);
		}

	}

	mPlayer->Update(dt);

	//mPlayer->SetCenter(mPlayer->Center() + 20 * mPlayer->mMoveSpeedScale * dt * moveVec);

	for (auto& e : mEnemies) {
		e->Update(dt);
	}

	for (auto& n : mNPCs) {
		n->Update(dt);
	}
 
	//
	// update missiles
	//
	for (auto it = mMissiles.begin(); it != mMissiles.end(); ) {
		Missile* m = *it;
		m->Update(dt);
		// check for collisions with player
		if (m->GetShooter() != mPlayer) {
			if (Dist(m->Center(), mPlayer->Center()) < m->Radius() + mPlayer->Radius()) {
				m->SetHealth(0);
				mPlayer->Damaged(10);
				AddEffect(new Effect(mExplosionTex, 16, 1.0f, false, m->Center()));
				// ignore collisions with player, for now
			}
		}

		// check for collisions with enemies
		for (auto e : mEnemies) {
			if (e->IsAlive() && m->GetShooter() != e && Dist(m->Center(), e->Center()) < m->Radius() + e->Radius()) {
				e->OnMissileImpact(m);
				m->SetHealth(0);
				AddEffect(new Effect(mExplosionTex, 16, 1.0f, false, m->Center()));
				if (e->IsDead()) {
					AddEffect(new Effect(mExplosionTex, 16, 1.0f, false, e->Center()));
				}
			}
		}

		//Check for c
		// remove missiles that leave world bounds or collide with stuff
		if (m->Health() <= 0 || m->Bottom() < 0 || m->Top() > mWorldHeight || m->Right() < 0 || m->Left() > mWorldWidth) {
			//delete *it;
			mMorgue.push_back(m);
			it = mMissiles.erase(it);
		}
		else {
			++it;
		}
	}

	// remove killed enemies
	for (auto it = mEnemies.begin(); it != mEnemies.end(); ) {
		if ((*it)->IsDead()) {
			mMorgue.push_back(*it);
			it = mEnemies.erase(it);
		}
		else {
			++it;
		}
	}

	//
	// apply non-penetration constraints
	//
	for (auto it1 = mEnemies.begin(); it1 != mEnemies.end(); ++it1) {
		Enemy* e = *it1;

		// check for collisions with player
		//closest enemy and player is the lowest value of all radius added together.
		float minSep = e->Radius() + mPlayer->Radius();
		float d = Dist(e->Center(), mPlayer->Center());
		if (d < minSep) {
			float depth = minSep - d;
			Vec2 fromPlayer = e->Center() - mPlayer->Center();
			fromPlayer.Normalize();
			Vec2 pos = e->Center() + depth * fromPlayer;
			e->SetCenter(pos);
		}



		// check for collisions with other enemies
		auto it2 = it1;
		for (++it2; it2 != mEnemies.end(); ++it2) {
			Enemy* e2 = *it2;
			float minSep = e->Radius() + e2->Radius();
			float d = Dist(e->Center(), e2->Center());
			if (d < minSep) {
				float depth = minSep - d;
				Vec2 axis = e->Center() - e2->Center();     // collision axis
				axis.Normalize();
				Vec2 pos1 = e->Center() + 0.5f * depth * axis;
				Vec2 pos2 = e2->Center() - 0.5f * depth * axis;
				e->SetCenter(pos1);
				e2->SetCenter(pos2);
			}
		}


		for (auto it3 = mBoundaries.begin(); it3 != mBoundaries.end(); ++it3) {
			Boundary* b = *it3;

			//check for collisions with boundaries
			if (b->mBoundaryRect->x < e->Right() &&
				b->mBoundaryRect->x + b->mBoundaryRect->w > e->Left() &&
				b->mBoundaryRect->y < e->Bottom() &&
				b->mBoundaryRect->y + b->mBoundaryRect->h > e->Top())
			{
				e->SetCenter(e->mPreviousPosition);
			}
		}
	}

 

	for (auto it3 = mBoundaries.begin(); it3 != mBoundaries.end(); ++it3) {
		Boundary* b = *it3;
		CheckCollisionWithBoundary(b, mPlayer);		
	}

	for (auto it3 = mNPCs.begin(); it3 != mNPCs.end(); ++it3) {
		NPC* n = *it3;
		CheckCollisionWithNPC(n, mPlayer);


	}
	

	ClipToWorldBounds(mPlayer);
	// ***************
	// **  CLIPPING **
	// ***************
	for (auto e : mEnemies) {
		ClipToWorldBounds(e);
	}

	// **********************

	// ******  CAMERA  ******
	// **********************

	int mx, my;
	SDL_GetMouseState(&mx, &my);
	//Vec2 mousePos = mCamera->ScreenToWorld(mx, my);
	//Vec2 toMouse = mousePos - mPlayer->Center();
	//Vec2 camPos = mPlayer->Center() + 0.3f * toMouse;
	//mCamera->LookAt(camPos);

	//
	// update all effects
	//
	for (auto it = mEffects.begin(); it != mEffects.end(); ) {
		Effect* e = *it;
		e->Update(dt);
		if (e->IsDone()) {

			delete e;
			it = mEffects.erase(it);
		}
		else {
			++it;
		}
	}

	//
	// clear the entity morgue
	//
	for (auto it = mMorgue.begin(); it != mMorgue.end(); ) {
		Entity* ent = *it;
		if (ent->mRefCount <= 0) {
			delete ent;
			it = mMorgue.erase(it);
		}
		else {
			++it;
		}
	}

	mIsActive = true;

	//updateBackground
	mCurrentBackground->AddTime(dt);

	// update camera
	mCamera->Update(dt);

}

void Gameplay::Draw(float dt)
{
	SDL_Renderer* renderer = mGame->GetRenderer();

	//mScriptProcessor_EffectsProcessActions(dt);
	//LAYER 0 (BACKGROUND)
	//SDL_Rect distantBackground;

	//mCamera->ScreenToWorld(0,0)
	//{(float) mCamera->WorldToScreenX(0),(float)mCamera->WorldToScreenY(0) }

	if (mDistantBackground != nullptr) {
		//parallax scale factors:
		float horizontalScale = 0.6f;
		float verticalScale = 0.6f;

		Vec2 pos(mCamera->WorldToScreenVec(0, 0).x * horizontalScale, mCamera->WorldToScreenVec(0, 0).y * verticalScale);
		mDistantBackground->Draw(renderer, pos, mCamera);
	}

	//currently the foreground:
	mCurrentBackground->Draw(renderer, mCamera->WorldToScreenVec(0, 0), mCamera);


	//entities have a layer property and we draw based on that. fkin simple




	//DEBUGGING SECTION START
	/*if (mCounter % 100 == 0) {*/
		//player center x : 1024
		//	player center y : 1684
		//	World To ScreenX(using 0 as position x and 4096) : -2231
		//	World To ScreenY(using 0 as position x and 3469) : -2418
		//	camera view left : 184
		//	camera view camera top : 684
		//	camera view bottom : 1734
		//	camera view right : 1864
	/*	mWorldWidth = 2048;
		mWorldHeight = 1734;*/

		//cout << "camera view left: " << mCamera->ViewLeft() << endl;
		//cout << "camera view camera top: " << mCamera->ViewTop() << endl;
		//cout << "camera view bottom: " << mCamera->ViewBottom() << endl;
		//cout << "camera view right: " << mCamera->ViewRight() << endl;

		//cout << "player center x: " << mPlayer->Center().x << endl;
		//cout << "player center y: " << mPlayer->Center().y << endl;

		//cout << "player x world to screen x: " << mCamera->WorldToScreenX(mPlayer->Center().x) << endl;
		//cout << "player y world to screen y: " << mCamera->WorldToScreenY(mPlayer->Center().y) << endl;

		//cout << "figaro Background animation x calculation: " << mCamera->WorldToScreenX(0 - 0.5f * 4096) << endl;
		//cout << "figaro Background animation y calculation: " << mCamera->WorldToScreenY(0 - 0.5f * 3468) << endl;

		//cout << "screen width " << (float)mGame->GetScreenWidth() << endl;
		//cout << "screen height: " << (float)mGame->GetScreenHeight() << endl;
		//cout << "*********************************************************" << endl;
		//cout << "*********************************************************" << endl;
		//cout << "camera left: " << mCamera->ScreenToWorld({ mPlayer->Center().x,mPlayer->Center().y }) << endl;

	//}
	//mCounter += 1;
	//DEBUGGING SECTION END

	/*

	backgrounds
	player
	npcs
	missiles
	enmies
	broundaries
	treses/treasure boxes

	*/

	//1. find max layer #. that is the upper bound
	//2. loop through all layers between that, 
	//3. fo

	//calculate
	//calculate layer order based on Y axis. 
	//sometimes we will just want to display effects at the foreground or background, that will need to be drawn outside of this logic or given negative or # > max layer #
	for (Entity* n : mDrawnObjects) {
		//convert y axis to a layer by multiplying its y value by -100!!!!!! so objects closer to the screen will be drawn last
		n->Layer() == ((int)n->Center().y) * 100;
	}


	std::vector<Entity*> x;
	int nextInteger = 0;


	for (list<NPC*>::iterator i = mNPCs.begin();
		i != mNPCs.end();
		i++) {
		int layerOrderNumber = i->Layer() == ((int)i->Center().y) * 100;
		i->Layer() == layerOrderNumber;

		for (Entity* n : mDrawnObjects) {
			if (layerOrderNumber >= n->Layer()) {
				//mDrawnObjects.splice at current position;

			}
		}

	} 

	for (Entity* n : mNPCs) {
		int layerOrderNumber = ((int)n->Center().y) * 100;
		n->Layer() == layerOrderNumber;
		//loop through mrawnObjects, and insert at the correct position. 
		for (Entity* n : mDrawnObjects) {
			if (layerOrderNumber >= n->Layer()){
				//mDrawnObjects.splice at current position;
			}
		}
		
	}

	for (Entity* n : mEnemies) {
		mDrawnObjects.push_back(n);
	}

	
	//this is very slow! First place to turn when performance becomes an issue
	for (Entity* n : mDrawnObjects) {
		if (n->Layer() == 1) {
			n->Draw(renderer, mCamera);
		}	
	}

	for (Entity* n : mDrawnObjects) {
		if (n->Layer() == 2) {
			n->Draw(renderer, mCamera);
		}
	}

	for (Entity* n : mDrawnObjects) {
		if (n->Layer() == 3) {
			n->Draw(renderer, mCamera);
		}
	}

	for (Entity* n : mDrawnObjects) {
		if (n->Layer() == 4) {
			n->Draw(renderer, mCamera);
		}
	}

	//for (auto& n : mNPCs) {
	//	n->Draw(renderer, mCamera);
	//}

	////if collide with a box, do not draw that part of the sprite. so it will make it seem like layers
	//mPlayer->Draw(renderer, mCamera);
	//for (auto& e : mEffects) {
	//	e->Draw(renderer, mCamera);
	//}




	////LAYER 1 (Second Background)
	//for (auto& e : mEnemies) {
	//	if (e->Entity::Layer() == 1) {
	//		e->Draw(renderer, mCamera);
	//	}
	//}

	//for (auto& m : mMissiles) {
	//	if (m->Entity::Layer() == 1) {
	//		m->Draw(renderer, mCamera);
	//	}
	//}

	if (mGame->mDebugMode) {
		for (auto& m : mBoundaries) {
			//m->mBoundaryRect->x = mCamera->WorldToScreenX(m->mBoundaryRect->x);
			//m->mBoundaryRect->y = mCamera->WorldToScreenY(m->mBoundaryRect->y);

			//cout << "boundary x draw: " << m->mBoundaryRect->x << endl;
			//cout << "boundary y draw: " << m->mBoundaryRect->y << endl;
			m->Draw(renderer, mCamera);
			//SDL_RenderFillRect(renderer, m->mBoundaryRect);
		}
	}
	/*
		SDL_Rect foregroundRect;
		foregroundRect.x = 0;
		foregroundRect.y = 0;
		SDL_QueryTexture(mNarsheForeground, NULL, NULL, &foregroundRect.w, &foregroundRect.h);
		SDL_RenderCopy(renderer, mNarsheForeground, &foregroundRect, NULL);
		*/
		//LAYER 2 (Main Level)

	//mPlayer2->Draw(renderer, mCamera);

	//for (auto& e : mEnemies) {
	//	if (e->Entity::Layer() == 2) {
	//		e->Draw(renderer, mCamera);
	//	}
	//}

	for (auto& m : mMissiles) {
		if (m->Entity::Layer() == 2) {
			m->Draw(renderer, mCamera);
		}
	}


	//Layer 4(next foreground)
	mScriptProcessor_CharacterMovements.ProcessActions(dt);

	//Layer 5 (Effects)
	//mCamera->LookAt(mPlayer->Center());

	mScriptProcessor_Effects.ProcessActions(dt);
}

void Gameplay::OnKeyDown(const SDL_KeyboardEvent& kbe)
{

	/*
		Generally, scancodes are the true values emitted by the keyboard (hardware)
		to the OS while keycode is what the OS/library maps it to based on the chosen layout.
		The layout decides the mapping between scancode to some virtual key code.
		It is part of the operating system's settings. Here, by layout, I mean the functional layout;
		there're also mechanical and visual layouts. Read more about keyboard layouts in Wikipedia.
		The concept of scan code and virtual key is explained better with illustration in MSDN.

		However, SDL uses scancode to mean something different: the scancode of the key in the
		US QWERTY keyboard whose location is the same as the one in question.

		It's the device-independant way of denoting a key based on its location.

		This is buried in an unusual location in SDL's manual: https://wiki.libsdl.org/MigrationGuide

	*/

	if (!mDialogueMode) {
		switch (kbe.keysym.scancode) {

		case SDL_SCANCODE_ESCAPE:
			std::cout << "User pressed Escape" << std::endl;
			//mGame->mShouldQuit = true;  // set quit flag
			mGame->EnterMainMenu();
			break;

		case SDL_SCANCODE_P:
			std::cout << mMissiles.size() << " missiles" << std::endl;
			break;

		case SDL_SCANCODE_V:
			g_EnableDebugVisualization ^= true;
			//mPlayer->mShowLocalAxes = !mPlayer->mShowLocalAxes;
			break;

		case SDL_SCANCODE_T:
			g_NoTarget ^= true;
			//mSound->PlaySFX(SoundConstants::S_WAV_FALLING, 1, 1);
			mGame->mDebugMode = !mGame->mDebugMode;

			break;
		case SDL_SCANCODE_SPACE:
			//Action key!  Open chest / Talk to player
			//
			for (auto& n : mNPCs) {
				Vec2 distDifference = mPlayer->Center() - n->Center();

				//cout << n->mName << ": distDifference.x " << distDifference.x << endl;
				//cout << n->mName << ": distDifference.y " << distDifference.y << endl;
			
				//

				if (abs(distDifference.x) < 30 && abs(distDifference.y) < 30) {
					mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, mGame->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), mGameRenderer, n->mDialogue, 5, mGame->mE, 2000, true));
				}
			}

			break;
		case SDL_SCANCODE_LSHIFT:
			mGameplayKeyboardHandler.btnPressed(SDL_SCANCODE_LSHIFT);
			break;

		case SDL_SCANCODE_A:
			mGameplayKeyboardHandler.btnPressed(SDL_SCANCODE_A);
			break;
		case SDL_SCANCODE_S:
			mGameplayKeyboardHandler.btnPressed(SDL_SCANCODE_S);
			break;
		case SDL_SCANCODE_D:
			mGameplayKeyboardHandler.btnPressed(SDL_SCANCODE_D);
			break;
		case SDL_SCANCODE_W:
			mGameplayKeyboardHandler.btnPressed(SDL_SCANCODE_W);
			break;
		case SDL_SCANCODE_J:
			mGameplayKeyboardHandler.btnPressed(SDL_SCANCODE_J);

			//SAVE BOUNDARIES TO THE FILE
			ofstream myfile;

			//Depending on which background we're working on. change this text file.
			// TODO: automate this to save to whichever is the current texture. can we find the name of the texture by querying it somehow?
			myfile.open("./media/figaroBoundaries.txt");
			//Save Boundaries
			for each (Boundary* var in mBoundaries)
			{
				//x,y,w,h,centerX,centerY				
				myfile << var->mBoundaryRect->x << "," << var->mBoundaryRect->y << "," << var->mBoundaryRect->w << "," << var->mBoundaryRect->h << "," << var->mCenter.x << "," << var->mCenter.y << endl;
			}
			myfile.close();
			break;

			//case SDL_SCANCODE_K:
			//	mGameplayKeyboardHandler.btnPressed(SDL_SCANCODE_K);
			//	//Load Boundaries
			//	//ofstream myfile;
			//	//Depending on which background we're working on. change this text file.
			//	// TODO: automate this to save to whichever is the current texture. can we find the name of the texture by querying it somehow?
			//	//myfile.open("./media/figaroBoundaries.txt");
			//	//*****************************************************************************
			//	//for each line in the file, split on commas and create a boundary object and throw it into the boundary array
			//	//*****************************************************************************

			//	//myfile.close();
			//	break;

		}
	}
}


void Gameplay::OnKeyUp(const SDL_KeyboardEvent& kbe)
{
	mGameplayKeyboardHandler.btnReleased(kbe.keysym.scancode);
}

void Gameplay::OnMouseUp(const SDL_MouseButtonEvent& mbe)
{
	if (mbe.button == SDL_BUTTON_RIGHT) {
		/*
			int x = mCamera->ScreenToWorldX(min(mTempBoundaryX, mbe.x) + (int)mCamera->ViewLeft());
			int y = mCamera->ScreenToWorldY(min(mTempBoundaryY, mbe.y) + (int)mCamera->ViewTop());
		*/
		int latestX = (int)mCamera->ScreenToWorldX(mbe.x);
		int latestY = (int)mCamera->ScreenToWorldY(mbe.y);

		int x = min(mTempBoundaryX, latestX);
		int y = min(mTempBoundaryY, latestY);
		//x = mCamera->WorldToScreenX(x);
		//y = mCamera->WorldToScreenY(y);
		//cout << x << endl;
		//cout << y << endl;

		cout << "min x : " << min(mTempBoundaryX, (int)mCamera->ScreenToWorldX(mbe.x)) << endl;
		cout << "min y : " << min(mTempBoundaryY, (int)mCamera->ScreenToWorldY(mbe.y)) << endl;
		cout << "mTempBoundaryX x create: " << mTempBoundaryX << endl;
		cout << "mTempBoundaryY y create: " << mTempBoundaryY << endl;
		cout << "boundary x create: " << x << endl;
		cout << "boundary y create: " << y << endl;

		Boundary* B = new Boundary(x, y, abs(mTempBoundaryX - latestX), abs(mTempBoundaryY - latestY));
		mBoundaries.push_back(B);
	}
}

void Gameplay::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
	if (mScriptProcessor_CharacterMovements.userControlEnabled && mGame->mScriptProcessor.userControlEnabled) {
		if (mbe.button == SDL_BUTTON_LEFT) {
			// shoot
			//std::cout << "Creating missile" << std::endl;

			////TODO: create constants for all textures
			//Missile* m = new Missile(mShotTex, mPlayer, 60, 3, true);
			//m->SetSpriteAngleCorrection(DIR_UP);
			//m->SetCenter(mPlayer->Center());
			//m->SetAngle(mPlayer->Angle());
			//m->SetSpeed(200);   // pixels per second

			/*
			//FOLLOWING CODE STARTS A FIREBALL ANIMATION :O

			//mMissiles.push_back(m); 
			SDL_Texture* fireball = ResourceManager::Acquire(PlayerConstants::BLAST, mGameRenderer);
			SDL_Texture* fireball2 = ResourceManager::Acquire(PlayerConstants::BLAST2, mGameRenderer);
			Vec2 targ(mbe.x, mbe.y);

			//change passing animations to chants then set inside the script processor :) 
			SDL_Texture* chantTex = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_CHANT, mGameRenderer);
			Animation* chant = new Animation(chantTex, 2, 0.4, true);
			SDL_Texture* castTex = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_SPELL_CAST, mGameRenderer);
			Animation* cast = new Animation(castTex, 1, 0.4, false);

			mGame->mScriptProcessor.AddAction(new aAction_BigBang(mPlayer, targ, fireball, fireball2, chant, cast, mGameRenderer, mCamera, mSound));
			*/
		}
		else if (mbe.button == SDL_BUTTON_RIGHT) {
			//create a box
			//when release. determine width/height of the box. 

			//create a new boundary object with that coordinate.
			//determine world coordinate from the window coordinate (is it world to begin with or view x/y??

			//boundary is basically a width and a height and a x and a y. 
			//maybe later we can extend it to be different shapes?

			//_x = mbe.x;
			//_y = mbe.y;
			mTempBoundaryX = mCamera->ScreenToWorldX(mbe.x);
			mTempBoundaryY = mCamera->ScreenToWorldY(mbe.y);
		}
	}
}

//
void Gameplay::ClipToWorldBounds(Entity* ent)
{

	if (ent->Left() < 0.0) {
		ent->SetLeft(0.0);
	}
	else if (mPlayer->Right() > mWorldWidth) {
		ent->SetRight((float)mWorldWidth);
	}
	if (mPlayer->Top() < 0.0) {
		ent->SetTop(0.0);
	}
	else if (mPlayer->Bottom() > mWorldHeight) {
		ent->SetBottom((float)mWorldHeight);
	}
}


void Gameplay::CheckCollisionWithBoundary(Boundary* b,Entity* e) {

	//check for collisions with boundaries
	if (b->mBoundaryRect->x < e->HitBoxRight() &&
		b->mBoundaryRect->x + b->mBoundaryRect->w > e->HitBoxLeft() &&
		b->mBoundaryRect->y < e->HitBoxBottom() &&
		b->mBoundaryRect->y + b->mBoundaryRect->h > e->HitBoxTop())
	{
		//if we are moving right, and the x is + 1 and the y is + 1. then only plus 1 
		//this logic makes the player slide along boundaries instead of "sticking" to the boundaries:
		Vec2 distDifference = e->Center() - e->mPreviousPosition;

		//heading right an down
		if (distDifference.x > 0.0f && distDifference.y > 0.0f) {

			//player is above the boundary, so slide along it
			if ((e->HitBoxBottom() - distDifference.y) < b->mBoundaryRect->y) {
				//Move left and right
				e->SetCenter(e->mPreviousPosition.x + abs(distDifference.x), e->mPreviousPosition.y);
			}
			//player is on the right side of the boundary, heading up:
			else {
				e->SetCenter(e->mPreviousPosition.x, e->mPreviousPosition.y + distDifference.y);
			}
		}
		//heading left an down
		else if (distDifference.x < 0.0f && distDifference.y > 0.0f) {

			//player is above the boundary, so slide along it
			if ((e->HitBoxBottom() - distDifference.y) < b->mBoundaryRect->y) {
				//Move left and right
				e->SetCenter(e->mPreviousPosition.x - abs(distDifference.x), e->mPreviousPosition.y);
			}

			else {
				e->SetCenter(e->mPreviousPosition.x, e->mPreviousPosition.y + distDifference.y);
			}

		}
		//heading right and up
		else if (distDifference.x > 0.0f && distDifference.y < 0.0f) {

			//player is below the boundary, heading up:
			if (e->mPreviousPosition.y > (b->mBoundaryRect->y + b->mBoundaryRect->h) && (e->mPreviousPosition.x + e->HitBoxRight()) > b->mBoundaryRect->x) {
				//Move right
				e->SetCenter(e->mPreviousPosition.x + abs(distDifference.x), e->mPreviousPosition.y);
			}
			//player is on the right side of the boundary, heading up:
			else {
				e->SetCenter(e->mPreviousPosition.x, e->mPreviousPosition.y - abs(distDifference.y));
			}
		}

		//heading left and up
		else if (distDifference.x < 0.0f && distDifference.y < 0.0f) {

			//if you are both below and to the right of the boundary, just revert to same position
			if (e->mPreviousPosition.y > (b->mBoundaryRect->y + b->mBoundaryRect->h) && e->mPreviousPosition.x > (b->mBoundaryRect->x + b->mBoundaryRect->w)) {
				e->SetCenter(e->mPreviousPosition.x, e->mPreviousPosition.y);
			}
			else if (e->mPreviousPosition.y > (b->mBoundaryRect->y + b->mBoundaryRect->h) && e->mPreviousPosition.x < (b->mBoundaryRect->x + b->mBoundaryRect->w)) {
				//Move left and right
				e->SetCenter(e->mPreviousPosition.x - abs(distDifference.x), e->mPreviousPosition.y);
			}
			// player is on the right side of the boundary, heading up, and the next step 
			// would bring the person inside the boundary, so keep outside the boundary, but slide along:
			else if (e->HitBoxLeft() < (b->mBoundaryRect->x + b->mBoundaryRect->w) && e->HitBoxTop() < (b->mBoundaryRect->y + b->mBoundaryRect->h)) {
				e->SetCenter(e->mPreviousPosition.x, e->mPreviousPosition.y - abs(distDifference.y));
			}
		}
		else {
			e->SetCenter(e->mPreviousPosition);
		}
		//  left or right?  if so, move up or down

		//e->SetCenter(e->mPreviousPosition.x, e->mPreviousPosition.y - abs(distDifference.y));
		//up or down
	}

	else {
		//e->SetCenter(e->mPreviousPosition);
	}
}


//See if entities collide
void Gameplay::CheckCollisionWithNPC(Entity* n, Entity* e) {
	//check for collisions with boundaries
	// rectangle hit box collision detection
	//if distance between is less than the
	cout << "test" << endl;
 	//Check collision with entities hitboxes, not their "texture radius" or sizes. 
	if (n->HitBoxLeft() < e->HitBoxRight() &&
		n->HitBoxRight() > e->HitBoxLeft() &&
		n->HitBoxTop() < e->HitBoxBottom() &&
		n->HitBoxBottom() > e->HitBoxTop())
	{
		cout << "test2" << endl;
		e->SetCenter(e->mPreviousPosition);

		////if we are moving right, and the x is + 1 and the y is + 1. then only plus 1 
		////this logic makes the player slide along boundaries instead of "sticking" to the boundaries:
		//Vec2 distDifference = e->Center() - e->mPreviousPosition;

		//bool collision = false;
		////heading right an down
		//if (distDifference.x > 0.0f && distDifference.y > 0.0f) {
		//	collision = true;		
		//}
		////heading left an down
		//else if (distDifference.x < 0.0f && distDifference.y > 0.0f) {		
		//	collision = true;
		//}
		////heading right and up
		//else if (distDifference.x > 0.0f && distDifference.y < 0.0f) {
		//	collision = true;
		//}
		////heading left and up
		//else if (distDifference.x < 0.0f && distDifference.y < 0.0f) {
		//	collision = true;
		//}		

		//if (collision == true) {
		//	
		//}
	}

	else {
		//e->SetCenter(e->mPreviousPosition);
	}
}