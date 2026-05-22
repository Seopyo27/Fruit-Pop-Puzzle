#include "GameApp.h"
#include <iostream>

int main()
{
	EHEngine::GameApp game;

	if (false == game.Initialize())
	{
		std::cout << "Failed to initialize the game. Error: " << GetLastError() << std::endl;
		return -1;
	}

	game.Run();

	game.Finalize();

	return 0;

}