#ifndef SOUND_H_
#define SOUND_H_

#include "./SDL_Mixer.h"

class Sound
{
	Mix_Music			*music;
	//The sound effects that will be used
	Mix_Chunk			*mLazer;
	Mix_Chunk			*mExplosion;
	Mix_Chunk			*med;
	Mix_Chunk			*low;

	int					mCurrentVolume;
public:
	Sound();
	~Sound();
	void				shutdown();
	void				playLazer();
	void				playExplosion();
	void				playTime();
	void				playAxelayTheme();
	void				stopMusic();
	void				increaseVolume();
	bool				Init();
};
#endif