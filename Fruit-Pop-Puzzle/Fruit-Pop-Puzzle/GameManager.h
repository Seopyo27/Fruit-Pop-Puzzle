#pragma once
#include "Script.h"
#include "Board.h"

namespace EHEngine
{
	class BoardManager;

	class GameManager : public Script
	{
	public:
		GameManager(GameObject* pOwner, GameApp* game) : Script(pOwner, game) {}

		void Start() override;
		void Update(float dT) override;

	private:
		enum class GameState
		{
			INITIALIZE,
			WAITING,
			SELECTED_FIRST_CELL,
			SWAP
		};
		GameState m_gameState = GameState::INITIALIZE;
		BoardManager* m_boardManager = nullptr;

		GridIndex m_firstSelected = { -1, -1 };
		GridIndex m_secondSelected = { -1, -1 };
	};

}


