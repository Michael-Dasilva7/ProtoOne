#ifndef SOUND_H_
#define SOUND_H_

#include "SDL_Mixer.h"
#include <string>
using std::string;

class Sound
{ 

	int					mCurrentVolume;
public:
	Sound();
	~Sound();
	void				shutdown();
	Mix_Music* pMus;
	void				playLazer();
	void				playExplosion();
	void				playTime(); 
	void				playMusicFadeIn(string name, int channel, int loops);
	void				stopMusic();
	void				increaseVolume();
	void				PlaySFX(string name, int channel, int loops);
	void				playMusic(int32_t channel, string sfxName, bool loop);

	bool				Init();
};
#endif