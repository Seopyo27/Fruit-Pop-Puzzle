#include "Core/GameApp.h"
#include <iostream>

int main()
{
	EHEngine::GameApp game;

	if (false == game.Initialize())
	{
		std::cout << "게임 초기화 실패" << std::endl;
		return -1;

	}

	game.Run();

	game.Finalize();

	return 0;
}