 
#ifndef LAYERMANAGER_H_
#define LAYERMANAGER_H_

#include <SDL.h>
#include "Texture.h"
#include <string>
#include <memory>
#include <unordered_map>


class LayerManager
{
	SDL_Texture*					mTex;

	float							mTime;              // current time position (in seconds)	
	//SDL_RenderCopyEx(renderer, mTex, &texRect, &screenRect, 0.0, NULL, mFlipType);
	
	
	//all we need to do is keep track of the render copy and render copy ex values.
	   // texture - texture.h
	  //RENDERER IS CONSTANT< DONT NEED TO SAVE IT
	//source rect -sdl.h
	//dest rect - sdl.h
	//fliptype - sdl.h
	//layer - int

	//texture, layer, src rect, dest rect
	//
public:
	//static std::shared_ptr<SDL_Texture> Acquire(const std::string& name, SDL_Renderer* renderer)
	static SDL_Texture* AcquireLayer(const std::string& name, SDL_Renderer* renderer)
	{
		const auto i = texturePtrs.find(name);
		if (i != texturePtrs.end())
		{
			return i->second;
		}
		else
		{
			//string dir = "./media";
			//auto pTex = std::make_shared<SDL_Texture>(LoadTexture(name.c_str, renderer));
			SDL_Texture* pTex = LoadTexture(name.c_str(), renderer);
			if (pTex != nullptr) {
				texturePtrs.insert({ name, pTex });
			}
			return pTex;
		}
	};

	static void renderAllLayers(){
		//Loop through list of layers and render them all. if they have a flip type is not null
		// then render EX. 
		//ELSE
		//Render normal
	}

	//static LayerManager(SDL_Texture* tex, int numCells, float duration, bool loopable, SDL_RendererFlip flipType = SDL_FLIP_NONE, int layerNumber);
	
	~LayerManager();
	SDL_RendererFlip				mFlipType;
	int								mCellWidth;
	int								mCellHeight;
	int								mNumCells;
	float							mDuration;          // in seconds
	bool							mIsLoopable;

	void							AddTime(float dt);

	bool							IsDone() const { return mTime >= mDuration; }

	 
	float							increaseAnimationSpeed(float addedDuration);

	SDL_Texture* 					GetCurrentTexture();

	void							SetCurrentTexture(SDL_Texture* tex);


private:
	static std::unordered_map<std::string, SDL_Texture*> texturePtrs;



};

#endif
