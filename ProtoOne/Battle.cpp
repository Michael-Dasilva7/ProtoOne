//
///*
//#include "Battle.h"
//#include "Texture.h"
//#include "Game.h"
//#include "Util.h"
//#include "GlobalsAreBad.h"
////#include "MonsterConstants.h"
//
//#include <iostream>
//#include <algorithm>
//
//Battle::Battle(Game* game, EnemyType monsterType, int level)
//	: GameState(game)
//{
//
//	/*
//	if level between 1-10 then this
//	if 10-20 this
//	if 20-30 this
//	if 30-35 this
//
//	*/
//}
//
//
//void Gameplay::Initialize()
//{
//	//main battle
//
//	SDL_Renderer* renderer = mGame->GetRenderer();
//	//
//	// load texture
//	//
//	
//	//load enemies passed in from 
//	//if StoryOneBoss then call resourceManager.LoadstoryOneBossTexture
//
//
//	//mPlayerTex = LoadTexture("media/shuttle.png", renderer);
//	mPlayerTex = LoadTexture("media/undineWalkLeft(4frame).png", renderer);
//	mShotTex = LoadTexture("media/shot.png", renderer);
//	mEnemyTex = LoadTexture("media/Spaceship-Drakir3.png", renderer);
//	mBgTex = LoadTexture("media/blah_dbg.jpg", renderer);
//	mExplosionTex = LoadTexture("media/explosion.tga", renderer);
//	//LoadLevel();
//	//ya you awnt a texture manager for sure...what if i wanna use this texture in another method/class? and i dont want gameplay to be the "boss"
//	//need to figure out how to animate flashes. utilize animation logic somehow..understand it
//	//mPlayerFlash 
//}
//
//void Gameplay::Shutdown()
//{
//	ClearLevel();
//
//	// delete the textures
//	SDL_DestroyTexture(mPlayerTex);
//	SDL_DestroyTexture(mShotTex);
//	SDL_DestroyTexture(mBgTex);
//	SDL_DestroyTexture(mEnemyTex);
//	SDL_DestroyTexture(mExplosionTex);
//
//
//}
//
//
//void Gameplay::LoadLevel()
//{
//	ClearLevel();
//
//	//
//	// initialize world
//	//
//
//	SDL_QueryTexture(mBgTex, NULL, NULL, &mWorldWidth, &mWorldHeight);
//
//	std::cout << "World size is " << mWorldWidth << "x" << mWorldHeight << std::endl;
//
//	//
//	// initialize the player
//	// 
//	mPlayer = new Player(mPlayerTex, 4, 0.1, true);
//	//create texture manager to create textures instead of the player/enemy because each one might be different
//	mPlayer->InitializePlayerTextures();
//	mPlayer->SetCenter(0.5f * mWorldWidth, 0.5f * mWorldHeight);
//	mPlayer->SetOrientation(DIR_DOWN);
//	mPlayer->SetSpriteAngleCorrection(DIR_UP);
//
//	//
//	// create some enemies
//	//
//
//	for (int i = 0; i < 1; i++) {
//		Enemy* e = new Enemy(mEnemyTex);
//		e->SetSpriteAngleCorrection(DIR_DOWN);
//		float x = RandomFloat(e->Radius(), mWorldWidth - e->Radius());
//		float y = RandomFloat(e->Radius(), mWorldHeight - e->Radius());
//		e->SetCenter(x, y);
//		e->SetAngle(RandomFloat(0.0f, 360.0f));
//		mEnemies.push_back(e);
//	}
//
//	Game* game = GetGame();
//
//	// create camera
//	mCamera = new Camera((float)game->GetScreenWidth(),
//		(float)game->GetScreenHeight(),
//		(float)mWorldWidth,
//		(float)mWorldHeight);
//
//	// make camera follow the player
//	//mCamera->SetTarget(mPlayer);
//}
//
//void Gameplay::ClearLevel()
//{
//
//	delete mPlayer;
//	mPlayer = NULL;
//
//
//	//Clear player textures:
//	//TODO: later on move this task to a resourceManager. 
//
//
//	for (auto it = mMissiles.begin(); it != mMissiles.end(); it++) {
//		Missile* m = *it;
//		delete m;
//	}
//	mMissiles.clear();
//
//	for (auto it = mEnemies.begin(); it != mEnemies.end(); it++) {
//		delete *it;
//	}
//	mEnemies.clear();
//
//	delete mCamera;
//	mCamera = NULL;
//}
//
//
//
////  
//void Gameplay::Update(float dt)
//{
//
//	mPlayer->Update(dt);
//
//	// update enemies
//	for (auto& e : mEnemies) {
//		e->Update(dt);
//	}
//
//	//
//	// update missiles
//	//
//	for (auto it = mMissiles.begin(); it != mMissiles.end(); ) {
//		Missile* m = *it;
//		m->Update(dt);
//		// check for collisions with player
//		if (m->GetShooter() != mPlayer) {
//			if (Dist(m->Center(), mPlayer->Center()) < m->Radius() + mPlayer->Radius()) {
//				m->SetHealth(0);
//				//mPlayer->SetState(PlayerState::DAMAGED);
//				mPlayer->Damaged(10);//Enemy.Attack *0.12 or whatever
//				AddEffect(new Effect(mExplosionTex, 16, 1.0f, m->Center()));
//				// ignore collisions with player, for now
//			}
//		}
//
//		// check for collisions with enemies
//		for (auto e : mEnemies) {
//			if (e->IsAlive() && m->GetShooter() != e && Dist(m->Center(), e->Center()) < m->Radius() + e->Radius()) {
//				e->OnMissileImpact(m);
//				m->SetHealth(0);
//				AddEffect(new Effect(mExplosionTex, 16, 1.0f, m->Center()));
//				if (e->IsDead()) {
//					AddEffect(new Effect(mExplosionTex, 16, 1.0f, e->Center()));
//				}
//			}
//		}
//
//		// remove missiles that leave world bounds or collide with stuff
//		if (m->Health() <= 0 || m->Bottom() < 0 || m->Top() > mWorldHeight || m->Right() < 0 || m->Left() > mWorldWidth) {
//			//delete *it;
//			mMorgue.push_back(m);
//			it = mMissiles.erase(it);
//		}
//		else {
//			++it;
//		}
//	}
//
//	// remove killed enemies
//	for (auto it = mEnemies.begin(); it != mEnemies.end(); ) {
//		if ((*it)->IsDead()) {
//			mMorgue.push_back(*it);
//			it = mEnemies.erase(it);
//		}
//		else {
//			++it;
//		}
//	}
//
//	//
//	// apply non-penetration constraints
//	//
//	for (auto it1 = mEnemies.begin(); it1 != mEnemies.end(); ++it1) {
//		Enemy* e = *it1;
//
//		// check for collisions with player
//		//closest enemy and player is the lowest value of all radius  added together.
//		float minSep = e->Radius() + mPlayer->Radius();
//		float d = Dist(e->Center(), mPlayer->Center());
//		if (d < minSep) {
//			float depth = minSep - d;
//			Vec2 fromPlayer = e->Center() - mPlayer->Center();
//			fromPlayer.Normalize();
//			Vec2 pos = e->Center() + depth * fromPlayer;
//			e->SetCenter(pos);
//		}
//
//		// check for collisions with other enemies
//		auto it2 = it1;
//		for (++it2; it2 != mEnemies.end(); ++it2) {
//			Enemy* e2 = *it2;
//			float minSep = e->Radius() + e2->Radius();
//			float d = Dist(e->Center(), e2->Center());
//			if (d < minSep) {
//				float depth = minSep - d;
//				Vec2 axis = e->Center() - e2->Center();     // collision axis
//				axis.Normalize();
//				Vec2 pos1 = e->Center() + 0.5f * depth * axis;
//				Vec2 pos2 = e2->Center() - 0.5f * depth * axis;
//				e->SetCenter(pos1);
//				e2->SetCenter(pos2);
//			}
//		}
//	}
//
//	//
//	// collisions with world bounds
//	//
//
//	ClipToWorldBounds(mPlayer);
//
//	for (auto e : mEnemies) {
//		ClipToWorldBounds(e);
//	}
//
//	//Vec2 camPos = mPlayer->Center();
//	//Vec2 camPos = mPlayer->Center() + 100 * mPlayer->ForwardDir();
//	int mx, my;
//	SDL_GetMouseState(&mx, &my);
//	Vec2 mousePos = mCamera->ScreenToWorld(mx, my);
//	Vec2 toMouse = mousePos - mPlayer->Center();
//	Vec2 camPos = mPlayer->Center() + 0.3f * toMouse;
//	mCamera->LookAt(camPos);
//
//	// update camera
//	mCamera->Update(dt);
//
//	//
//	// update all effects
//	//
//	for (auto it = mEffects.begin(); it != mEffects.end(); ) {
//		Effect* e = *it;
//		e->Update(dt);
//		if (e->IsDone()) {
//			delete e;
//			it = mEffects.erase(it);
//		}
//		else {
//			++it;
//		}
//	}
//
//	//
//	// clear the entity morgue
//	//
//	for (auto it = mMorgue.begin(); it != mMorgue.end(); ) {
//		Entity* ent = *it;
//		if (ent->mRefCount <= 0) {
//			delete ent;
//			it = mMorgue.erase(it);
//		}
//		else {
//			++it;
//		}
//	}
//
//	mIsActive = true;
//}
//
//void Gameplay::Draw()
//{
//	SDL_Renderer* renderer = mGame->GetRenderer();
//
//	SDL_Rect bgRect;
//	bgRect.x = RoundToInt(mCamera->ViewLeft());
//	bgRect.y = RoundToInt(mCamera->ViewTop());
//	bgRect.w = mGame->GetScreenWidth();
//	bgRect.h = mGame->GetScreenHeight();
//
//	SDL_RenderCopy(renderer, mBgTex, &bgRect, NULL);
//
//	// draw missiles
//	for (auto& m : mMissiles) {
//		m->Draw(renderer, mCamera);
//	}
//
//	// draw enemies
//	for (auto& e : mEnemies) {
//		e->Draw(renderer, mCamera);
//	}
//
//	// draw player
//	mPlayer->Draw(renderer, mCamera);
//
//	// draw effects
//	for (auto& e : mEffects) {
//		e->Draw(renderer, mCamera);
//	}
//}
//
//void Gameplay::OnKeyDown(const SDL_KeyboardEvent& kbe)
//{
//	switch (kbe.keysym.sym) {
//	case SDLK_ESCAPE:
//		std::cout << "User pressed Escape" << std::endl;
//		//mGame->mShouldQuit = true;  // set quit flag
//		mGame->EnterMainMenu();
//		break;
//
//	case SDLK_p:
//		std::cout << mMissiles.size() << " missiles" << std::endl;
//		break;
//
//	case SDLK_v:
//		g_EnableDebugVisualization ^= true;
//		//mPlayer->mShowLocalAxes = !mPlayer->mShowLocalAxes;
//		break;
//
//	case SDLK_t:
//		g_NoTarget ^= true;
//		break;
//
//	case SDLK_LSHIFT:
//		mPlayer->mMoveSpeedScale = 2.0f;
//		break;
//	}
//}
//
//
//void Gameplay::OnKeyUp(const SDL_KeyboardEvent& kbe)
//{
//	switch (kbe.keysym.sym) {
//	case SDLK_LSHIFT:
//		mPlayer->mMoveSpeedScale = 1.0f;
//		break;
//	}
//}
//
//void Gameplay::OnMouseDown(const SDL_MouseButtonEvent& mbe)
//{
//	if (mbe.button == SDL_BUTTON_LEFT) {
//		// shoot
//		std::cout << "Creating missile" << std::endl;
//
//		Missile* m = new Missile(mShotTex, mPlayer);
//		m->SetSpriteAngleCorrection(DIR_UP);
//		m->SetCenter(mPlayer->Center());
//		m->SetAngle(mPlayer->Angle());
//		m->SetSpeed(500);   // pixels per second
//
//		mMissiles.push_back(m);
//	}
//}
//
//void Gameplay::ClipToWorldBounds(Entity* ent)
//{
//	if (ent->Left() < 0) {
//		ent->SetLeft(0);
//	}
//	else if (mPlayer->Right() > mWorldWidth) {
//		ent->SetRight((float)mWorldWidth);
//	}
//	if (mPlayer->Top() < 0) {
//		ent->SetTop(0);
//	}
//	else if (mPlayer->Bottom() > mWorldHeight) {
//		ent->SetBottom((float)mWorldHeight);
//	}
//}
