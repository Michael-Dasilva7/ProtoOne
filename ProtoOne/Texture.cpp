#include <iostream>

#include <SDL_image.h>

#include "Texture.h"

SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer)
{

    // load an image into an SDL_Surface (stored in RAM)
    SDL_Surface* surf = IMG_Load(path);                                                     // <--- ###
    if (!surf) {
        std::cerr << "*** Failed to load image: " << IMG_GetError() << std::endl;
        return NULL;
    }

    // create an SDL_Texture from the surface (stored in GPU DRAM)
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);                        // <--- ###
    if (!tex) {
        std::cerr << "*** Failed to create texture: " << SDL_GetError() << std::endl;
    }

    // delete the surface (no longer needed)
    SDL_FreeSurface(surf);                                                                  // <--- ###

    return tex;
}

//SDL_Texture* LoadTextureArea(const char* path, SDL_Renderer* renderer, int x, int y) {
//	 /*
//	SDL_TEXTUREACCESS_STATIC
//
//		changes rarely, not lockable
//
//	SDL_TEXTUREACCESS_STREAMING
//
//		changes frequently, lockable
//
//	SDL_TEXTUREACCESS_TARGET
//
//		can be used as a render target
//	*/
//	SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, ,
//			int h);
//}