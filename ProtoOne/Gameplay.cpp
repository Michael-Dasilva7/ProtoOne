#include "Gameplay.h"
#include "Game.h"
#include "Util.h"
#include "GlobalsAreBad.h" 
#include "ResourceManager.h"
#include "StoryScriptConstants.h"
#include "Actions.h"
#include <iostream>
#include <algorithm>

Gameplay::Gameplay(Game* game)
	: GameState(game)
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
	//,mResourceManager(mGame->GetResourceManager())
	, mGameRenderer(mGame->GetRenderer())
	//, mScript(&mGame->mScriptProcessor)
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
	mShotTex = ResourceManager::Acquire("media/shot.png", renderer);

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

	// delete the textures
	SDL_DestroyTexture(mPlayerTex);
	SDL_DestroyTexture(mShotTex);
	SDL_DestroyTexture(mNarsheForeground);
	SDL_DestroyTexture(mNarsheBackground);
	SDL_DestroyTexture(mEnemyTex);
	SDL_DestroyTexture(mExplosionTex);

	//textureManager destroyActiveTextures :)

}


void Gameplay::LoadLevel()
{

	SDL_Renderer* renderer = mGame->GetRenderer();
	//ClearLevel();
	//
	// initialize world
	//
	SDL_QueryTexture(mNarsheForeground, NULL, NULL, &mWorldWidth, &mWorldHeight);
	mWorldWidth = mWorldWidth;
	mWorldHeight = mWorldHeight;
	//std::cout << "World size is " << mWorldWidth  << "x" << mWorldHeight << std::endl;

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

	mPlayer = new Player(ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_UP, mGameRenderer), 4, 1, true);
	mPlayer->SetCenter(mWorldWidth - 420, mWorldHeight);
	mPlayerTex = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_WALK_UP, renderer);
	mPlayer->mWalkDownTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_WALK_DOWN, renderer);
	mPlayer->mWalkUpTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_WALK_UP, renderer);
	mPlayer->mWalkLeftTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_WALK_LEFT, renderer);
	mPlayer->mWalkRightTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_WALK_RIGHT, renderer);
	mPlayer->mRunDownTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_DOWN, renderer);
	mPlayer->mRunUpTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_UP, renderer);
	mPlayer->mRunLeftTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_LEFT, renderer);
	mPlayer->mRunRightTexture = ResourceManager::Acquire(PlayerConstants::ANIM_TEX_UNDINE_RUN_RIGHT, renderer);

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
	
	Enemy* e = new Enemy(ResourceManager::Acquire(EnemyConstants::GIGA_GAIA_IDLE, renderer));
	float x = 1000;
	float y = 1000;
	e->SetCenter(x, y);
	e->SetLayer( 1);
	e->SetState(ENEMY_HOVER);
	e->SetSpeedScale(0);
	mEnemies.push_back(e);


	Game* game = GetGame();

	mCamera = new Camera((float)game->GetScreenWidth(),
		(float)game->GetScreenHeight(),
		(float)mWorldWidth,
		(float)mWorldHeight);

	// make camera follow the player
	mCamera->SetTarget(mPlayer);

	/*
	// __START FIRST CUTSCENE___
	//	talk to the king, get orders.. lightning.. rain
	//   train whistle
	// start train sound....
	*/
   
	float newplayerX;
	float newplayerY;

	ResourceManager::initializeTextBoxTextures(renderer);
	ResourceManager::PopulateCharacterRects();
	 
	mGame->mScriptProcessor.AddAction(new aAction_FadeIn(game->GetScreenWidth(), game->GetScreenHeight(), 1000, mGame->GetRenderer(),0,0,0));
	mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE, 45));

	mScriptProcessor_Effects.AddAction(new aAction_FadeIn(game->GetScreenWidth(), game->GetScreenHeight(), 1000, mGame->GetRenderer(), 0, 0, 0));
	

	//mGame->mScriptProcessor(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE, 45));
	
	//start in the bottom right of the map and walk up to the bridge.
	newplayerY = mPlayer->Top() - 840;
	newplayerX = mPlayer->Center().x;
	mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, Vec2(newplayerX, newplayerY), 55.0f));

	newplayerX -= 600;
	mScriptProcessor_CharacterMovements.AddAction(new aAction_MoveTo(mPlayer, Vec2(newplayerX, newplayerY), 55.0f));

	//TODO4: when walk/run button is released, whatever direction the player is currently facing, determines which idle animation to show. 
	//       we either want another 1 frame texture, or just display the first frame of the walk animation
	//TODO5: edit action or create new action to change the amount of time a movement takes. either LERP it, or choose walk or run maybe. 
	//        OR the best is probably pass in the movement speed of the player!! ez. but need t change back probably for when user gets control. but this cutscene wont have control
	//     after cutscene a battle should start. and text boxes.
	
	//FLASH!

	//or just create a dynamic action that makes everything do everyhting needed in the order! 

	//can add flash after a specific action if i put it into a different script processor
	//
	mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::AFTER_FLASH, 45));

	//newplayerX -= 400;
	//mGame->mScriptProcessor.AddAction(new aAction_MoveTo(mPlayer, Vec2(newplayerX, newplayerY), 3.0f));
	//mGame->mScriptProcessor.AddAction(new aAction_MoveTo(mPlayer, Vec2(newplayerX - 70, newplayerY), 3.0f));
	//mGame->mScriptProcessor.AddAction(new aAction_MoveTo(mPlayer, Vec2(newplayerX - 70, newplayerY - 400), 3.0f));

	//mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::DRAMA_AT_THE_TOP_OF_THE_CLIFF, 85));
	//mGame->mScriptProcessor.AddAction(new aAction_Dialogue(0, 0, game->GetScreenWidth(), 300, mTextBoxFF6, mTextImage, ResourceManager::getTexturePtrList(), renderer, StoryScriptConstants::INTRO_SEQUENCE, 85));

	//1. WIND HOWLS
	//2. Screen fades in
	//3. snow is blowing
	//4. text box "i am sensing an energy field and wanted to track the signal. seemed to have come from all over
	//   " its.....
	//   " its... coming from...above!!
	//*shooting start across the sky*
	//CRASH
	//FLASH
	//Explosion
	//i teleport to the location.
	//find the gem, it glows and floats
	//i get attacked from a possessed human.
	//crystal possesses people and gives them power. the crystal is sentient. evil. creshinibon
	//CODING TRAIN SNOW.

	//NINJA ATTACK AND I FIRE BLAST THEM AWAY AND THE SNOW TEMPORARILY.

	//THEN ANOTHER BOSS NINJA COMES AND ITS THE FIRST BATTLE THAT I CONTROL THE PLAYER.
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



//  
void Gameplay::Update(float dt)
{
	//std::cout << "World Width: "<< mWorldWidth << std::endl;
	//std::cout << "World Height: " << mWorldHeight << std::endl;
	const Uint8* keys = mGame->GetKeys();
	//const Uint8* keyState = mGame->GetGameplayState();	


	//mScript.ProcessActions(dt);

	mPlayer->Update(dt);
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

	Vec2 moveVec;
	//if (mScript->userControlEnabled) {
	if(mScriptProcessor_CharacterMovements.userControlEnabled) {
		//priority order: u,left,right,down
		if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_A)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running
			//we want to swap between current chars with "R". so 						
			//load texture returns a pointer...so what, were gonna have a pointer to a pointer? hmm probably a better way.			
			mPlayer->moveLeft(moveVec, dt, false);
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_A);
		}
		else if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_S)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running			
			mPlayer->moveDown(moveVec, dt, false);
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_S);

		}
		else if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_D)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running			
			mPlayer->moveRight(moveVec, dt, false);
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_D);

		}
		else if (mGameplayKeyboardHandler.isReleased(SDL_SCANCODE_W)) {
			//SET IDLE LEFT TEXTURE so we dont have a frozen run animation, and it looks ok when we stop walking/running			
			mPlayer->moveUp(moveVec, dt, false);
			mGameplayKeyboardHandler.setIdleKeyState(SDL_SCANCODE_W);
		}

		if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_A)) {
			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				moveVec = mPlayer->moveLeft(moveVec, dt, true);
			}
			else {
				moveVec = mPlayer->moveLeft(moveVec, dt, false);
			}
			//TODO: make an enemy character float with shadows behind him. and certain atacks make him glow colors,  very similar to ASRIEL DREEMUR					
		}
		else if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_W)) {
			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				//RUN				
				moveVec = mPlayer->moveUp(moveVec, dt, true);
			}
			else {
				//WALK				+
			 	moveVec = mPlayer->moveUp(moveVec, dt, false);
			}
		}
		else if (keys[SDL_SCANCODE_S]) {

			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				//RUN
				moveVec = mPlayer->moveDown(moveVec, dt, true);
			}
			else {
				//WALK
				moveVec = mPlayer->moveDown(moveVec, dt, false);
			}
		}

		else if (keys[SDL_SCANCODE_D]) {
			if (mGameplayKeyboardHandler.isPressed(SDL_SCANCODE_LSHIFT)) {
				//RUN
				moveVec = mPlayer->moveRight(moveVec, dt, true);
			}
			else {
				//WALK
				moveVec = mPlayer->moveRight(moveVec, dt, false);
			}
		}
		else if (keys[SDL_SCANCODE_U]) {
			mPlayer->Entity::mCurrentAnimation->increaseAnimationSpeed(0.01);
		}
		else if (keys[SDL_SCANCODE_SPACE]) {
			//mPlayer->Entity::mCurrentAnimation->
			
			//Check if we are facing an NPC, and if so, bring up dialogue
			int width = mGame->GetScreenWidth();
			int height = mGame->GetScreenHeight();
			mScriptProcessor_Effects.AddAction(new aAction_FadeIn(width, height, 1000, mGame->GetRenderer(), 255, 255, 230));
			mScriptProcessor_Effects.AddAction(new aAction_FadeIn(width, height, 800, mGame->GetRenderer(), 255, 255, 230));
			mScriptProcessor_Effects.AddAction(new aAction_FadeIn(width, height, 800, mGame->GetRenderer(), 255, 255, 230));
			mScriptProcessor_Effects.AddAction(new aAction_FadeIn(width, height, 800, mGame->GetRenderer(), 255, 255, 230));
		 
		}
		else {

		}

	}
		mPlayer->SetCenter(mPlayer->Center() + 20 * mPlayer->mMoveSpeedScale * dt * moveVec);

		// update enemies
		for (auto& e : mEnemies) {
			e->Update(dt);
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
					AddEffect(new Effect(mExplosionTex, 16, 1.0f, m->Center()));
					// ignore collisions with player, for now
				}
			}

			// check for collisions with enemies
			for (auto e : mEnemies) {
				if (e->IsAlive() && m->GetShooter() != e && Dist(m->Center(), e->Center()) < m->Radius() + e->Radius()) {
					e->OnMissileImpact(m);
					m->SetHealth(0);
					AddEffect(new Effect(mExplosionTex, 16, 1.0f, m->Center()));
					if (e->IsDead()) {
						AddEffect(new Effect(mExplosionTex, 16, 1.0f, e->Center()));
					}
				}
			}

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

		// update camera
		mCamera->Update(dt);

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


}

void Gameplay::Draw(float dt)
{

	//depending on the location, we need to display a different background:
	SDL_Renderer* renderer = mGame->GetRenderer();

	//so each loop cycle is unique to the area. if its narshe area 1, we have a different layer set...
	
	//area is probably best as a string. eventually if we have tons of areas we want to make this 
	//easily scalable.... for now lets hardcode. layer we probably want different game states.

	//battle state. maybe other states depending on the type of gameplay thats going on
 
	//LAYER 0 (BACKGROUND)
	SDL_Rect distantBackground;
	distantBackground.x = 0;//RoundToInt(mCamera->ViewLeft());
	distantBackground.y = 0;//RoundToInt(mCamera->ViewTop());
	distantBackground.w = mGame->GetScreenWidth();
	distantBackground.h = mGame->GetScreenHeight();
	SDL_RenderCopy(renderer, mNarsheBackground, &distantBackground, NULL);


	//LAYER 1 (Second Background)


	for (auto& e : mEnemies) {
		if (e->Entity::Layer() == 1) {
			e->Draw(renderer, mCamera);
		}
	}

	for (auto& m : mMissiles) {
		if (m->Entity::Layer() == 1) {
			m->Draw(renderer, mCamera);
		}
	}

	SDL_Rect foregroundRect;
	foregroundRect.x = RoundToInt(mCamera->ViewLeft());
	foregroundRect.y = RoundToInt(mCamera->ViewTop());
	foregroundRect.w = mGame->GetScreenWidth();
	foregroundRect.h = mGame->GetScreenHeight();
	cout << "foreground X: " << foregroundRect.x << endl;
	cout << "foreground Y: " << foregroundRect.y << endl;
	SDL_RenderCopy(renderer, mNarsheForeground, &foregroundRect, NULL);

	//LAYER 2 (Main Level)
	
	
	mPlayer->Draw(renderer, mCamera);
	
	for (auto& e : mEnemies) {
		if (e->Entity::Layer() == 2) {
			e->Draw(renderer, mCamera);
		}
	}

	for (auto& m : mMissiles) {
		if (m->Entity::Layer() == 2) {
			m->Draw(renderer, mCamera);
		}
	}

	//add an action to 
	//TODO: find out why we canot click to go through dialogue now.
	// make animation
	//make giga gaia stay still and appear once i get to the top and slowly make an entrance
	//add sound/music
	//attacks./

	//add snow
	//change missiles to default to layer 2

	//LAYER 3 (Foreground)

	

	//mScript.ProcessActions(dt);
	//LAyer 4(next foreground)


	//LAyer 5 (Effects)
	mScriptProcessor_Effects.ProcessActions(dt);

	for (auto& e : mEffects) {
		e->Draw(renderer, mCamera);
	}

	//std::string x = "MADCODE";
	//for (char& c : x) {
	//	//src rect is where in the texture you want to grab
	//	//dest rect is where on the screen you want to display it
	//	SDL_Rect textRect; 
	//	textRect.w = 8;
	//	textRect.h = 8;

	//	int v1 = (((int)c) % 65);
	//	if (v1 >= 0 && v1 <= 15) {
	//		textRect.y = 0;
	//		textRect.x = v1 * 8;
	//	}

	//	currentLetterXPos += 8;
	//	SDL_Rect destRect;
	//	destRect.w = 8;
	//	destRect.h = 8;
	//	destRect.x = currentLetterXPos;
	//	destRect.y = 0;
	//	  
	//	//SDL_RenderCopy will return 0 on success, or -1 on error
	//	SDL_RenderCopy(renderer, textImage, &textRect, &destRect);
 //
	//}	 

	//SDL_Rect srcRect;
	//srcRect.w = 8;
	//srcRect.h = 8;
	//srcRect.x = 0;
	//srcRect.y = 0;

	//SDL_Rect destRect;
	//destRect.x = 0;
	//destRect.y = 0;
	//destRect.w = 15;
	//destRect.h = 15;
	//int outcome =  SDL_RenderCopy(renderer, textImage, &srcRect, &destRect);

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


		}
	}

}


void Gameplay::OnKeyUp(const SDL_KeyboardEvent& kbe)
{
	mGameplayKeyboardHandler.btnReleased(kbe.keysym.scancode);
}


void Gameplay::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
	if (mbe.button == SDL_BUTTON_LEFT) {
		// shoot
		std::cout << "Creating missile" << std::endl;

		Missile* m = new Missile(mShotTex, mPlayer);
		m->SetSpriteAngleCorrection(DIR_UP);
		m->SetCenter(mPlayer->Center());
		m->SetAngle(mPlayer->Angle());
		m->SetSpeed(200);   // pixels per second

		mMissiles.push_back(m);
	}
}

//
void Gameplay::ClipToWorldBounds(Entity* ent)
{
	if (ent->Left() < 0) {
		ent->SetLeft(0);
	}
	else if (mPlayer->Right() > mWorldWidth) {
		ent->SetRight((float)mWorldWidth);
	}
	if (mPlayer->Top() < 0) {
		ent->SetTop(0);
	}
	else if (mPlayer->Bottom() > mWorldHeight) {
		ent->SetBottom((float)mWorldHeight);
	}
}
