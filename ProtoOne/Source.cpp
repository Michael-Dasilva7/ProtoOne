
#include "Sound.h"
#include "SoundConstants.h"
#include "SoundManager.h"

Sound::Sound()
{
	//since we are using shared pointers for music and SFX, do we need FREE MUS AND FREE CHUNK  to garage collect sounds and music???
}

bool Sound::Init()
{
	// Looking for music? Take a look at my library here:
	mCurrentVolume = 2;

	bool success = true;
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	else {
		Mix_Volume(-1, 1);
		Mix_VolumeMusic(25);
	}

	return success;
}

/* Play an audio chunk on a specific channel.
   If the specified channel is -1, play on the first free channel.
   If 'loops' is greater than zero, loop the sound that many times.
   If 'loops' is -1, loop inifinitely (~65000 times).
   Returns which channel was used to play the sound.
*/
void Sound::playMusicFadeIn(string name, int loops, int fadeInMS)
{ 
	Mix_FadeInMusic(SoundManager::AcquireMus(SoundConstants::FOLDER_MUS.c_str() + name), loops, fadeInMS);
}

/* Play an audio chunk on a specific channel.
	   If the specified channel is -1, play on the first free channel.
	   If 'loops' is greater than zero, loop the sound that many times.
	   If 'loops' is -1, loop inifinitely (~65000 times).
	   Returns which channel was used to play the sound.
*/
void Sound::PlaySFX(string name, int channel, int loops) {

	Mix_PlayChannel(-1, SoundManager::Acquire(SoundConstants::FOLDER_SFX.c_str() + name), 0);
}


//void Sound::PlaySoundEffect(int channel) {
//	//mChannelOneSFX = Mix_LoadWAV("media/SFX/BAFallingDown.wav");
//}

//void Sound::LoadChannelOne() {
//	
//	//mFalling = Mix_LoadWAV("media/SFX/BAFallingDown.wav");
//}

void Sound::increaseVolume()
{
	Mix_Volume(-1,mCurrentVolume++);

}
//void Sound::stopMusic()
//{
//	Mix_FreeMusic(music);
//	music = NULL;
//}

void Sound::shutdown()
{
	//Mix_FreeChunk(mLazer);
	//mLazer = NULL;//to be safe
	//Mix_FreeChunk(mExplosion);
	//mExplosion = NULL;//to be safe
	//Mix_FreeChunk(med);
	//med = NULL;//to be safe
	//Mix_FreeChunk(low);
	//low = NULL;//to be safe
	
	while (Mix_Init(0))
	{
		Mix_Quit();
	}//make sure mixer is unloaded from memory
}