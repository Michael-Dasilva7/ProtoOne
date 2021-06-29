#include "Sound.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>

Sound::Sound()
{
	//call init before playing sounds
}

bool Sound::Init()
{
	// Looking for music? Take a look at my library here:


	mCurrentVolume = 0;
	bool success = true;
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	mLazer = Mix_LoadWAV("media/lazer1.wav");
	if (mLazer == NULL)
	{
		printf("Failed to load lazer SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	mExplosion = Mix_LoadWAV("media/explode.wav");
	if (mExplosion == NULL)
	{
		printf("Failed to load explosion! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	return success;
}
void Sound::playLazer()
{
	Mix_PlayChannel(1, mLazer, 0);
}
void Sound::playExplosion()
{
	Mix_PlayChannel(-1, mExplosion, 0);
}
void Sound::playTime()
{

	music = Mix_LoadMUS("media/time.mp3");
	Mix_FadeInMusic(music, -1, 4000);
}
void Sound::playAxelayTheme()
{
	music = Mix_LoadMUS("media/time.mp3");
	Mix_FadeInMusic(music, -1, 4000);
//	Mix_PlayMusic(music, -1);
}

void Sound::increaseVolume()
{
	Mix_Volume(-1,mCurrentVolume++);
}
void Sound::stopMusic()
{
	Mix_FreeMusic(music);
	music = NULL;
}
void Sound::shutdown()
{
	Mix_FreeChunk(mLazer);
	mLazer = NULL;//to be safe
	Mix_FreeChunk(mExplosion);
	mExplosion = NULL;//to be safe
	Mix_FreeChunk(med);
	med = NULL;//to be safe
	Mix_FreeChunk(low);
	low = NULL;//to be safe

	while (Mix_Init(0))
	{
		Mix_Quit();
	}//make sure mixer is unloaded from memory
}