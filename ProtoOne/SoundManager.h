#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include "SDL_Mixer.h"
#include <string>
#include <memory>
#include <unordered_map>
class SoundManager {

public:
	//static std::shared_ptr<SDL_Texture> Acquire(const std::string& name, SDL_Renderer* renderer)
	static Mix_Chunk* Acquire(const std::string& name);
	static Mix_Music* AcquireMus(const std::string& name);
private:
	 static std::unordered_map<std::string, Mix_Chunk*> chunkPtrs;
	 static std::unordered_map<std::string, Mix_Music*> musPtrs;

};


#endif