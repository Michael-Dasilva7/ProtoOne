#include "SoundManager.h"

std::unordered_map<std::string, Mix_Chunk*> SoundManager::chunkPtrs;
std::unordered_map<std::string, Mix_Music*> SoundManager::musPtrs;

 Mix_Chunk* SoundManager::Acquire(const std::string& name) {
	const auto i = chunkPtrs.find(name);
	if (i != chunkPtrs.end())
	{
		return i->second;
	}
	else
	{
		Mix_Chunk* pChunk = Mix_LoadWAV(name.c_str());
		if (pChunk == NULL)
		{
			printf("Failed to load SFX. Mix error: %s\n", Mix_GetError());

		}
		if (pChunk != nullptr) {
			chunkPtrs.insert({ name, pChunk });
		}
		return pChunk;
	}
};

 Mix_Music* SoundManager::AcquireMus(const std::string& name) {
	 const auto i = musPtrs.find(name);
	 if (i != musPtrs.end())
	 {
		 return i->second;
	 }
	 else
	 {
		 //auto pTex = std::make_shared<SDL_Texture>(LoadTexture(name.c_str, renderer));
		 Mix_Music* pMus = Mix_LoadMUS(name.c_str());
		 if (pMus == NULL)
		 {
			 printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());

		 }
		 if (pMus != nullptr) {
			 musPtrs.insert({ name, pMus });
		 }
		 return pMus;
	 }
 };