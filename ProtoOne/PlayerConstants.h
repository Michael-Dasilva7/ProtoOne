#pragma once

#include <string>

class PlayerConstants
{	
public:

	const static int PlayerConstants::MAX_LEVEL = 99;
	const static int PlayerConstants::STARTING_HEALTH = 999;
	const static int PlayerConstants::RUN_SPEED_MODIFYER = 3;
	const static int PlayerConstants::WALK_SPEED_MODIFYER = 2;


	const static std::string PlayerConstants::ANIM_TEX_UNDINE_WALK_LEFT;
	const static std::string PlayerConstants::ANIM_TEX_UNDINE_WALK_UP;
	const static std::string PlayerConstants::ANIM_TEX_UNDINE_WALK_RIGHT;
	const static std::string PlayerConstants::ANIM_TEX_UNDINE_WALK_DOWN;

	const static std::string PlayerConstants::ANIM_TEX_UNDINE_RUN_LEFT;
	const static std::string PlayerConstants::ANIM_TEX_UNDINE_RUN_UP;
	const static std::string PlayerConstants::ANIM_TEX_UNDINE_RUN_RIGHT;
	const static std::string PlayerConstants::ANIM_TEX_UNDINE_RUN_DOWN;


	//const static PlayerConstants::RetrieveAnimationValues(string& name) {
	//	switch (name) {
	//		case PlayerConstants::ANIM_TEX_UNDINE_WALK_LEFT:
	//			
	//			break;
	//		case PlayerConstants::ANIM_TEX_UNDINE_WALK_UP:
	//			break;
	//		default:
	//	}
	//	return PlayerTextureFileDirectory + name;
	//}	

};