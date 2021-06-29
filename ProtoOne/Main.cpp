#include <iostream>
#include <SDL.h>

#include "Game.h"
#include "Util.h"

Game g_Game;

Game* GetGame()
{
    return &g_Game;
}


int main(int argc, char** argv)
{
	std::cout << "Hello" << std::endl;

    InitRandom();

	if (!g_Game.Run()) {
		std::cerr << "*** Something horrible has happened" << std::endl;
		return 1;
	}

    std::cout << "Goodbye" << std::endl;

	return 0;
}
