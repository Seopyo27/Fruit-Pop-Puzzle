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
			NONE,
			INITIALIZE,
			WAITING,
			SELECTED_FIRST_CELL,
			SWAP,
			MATCH,
			POP,
			DROP,
			SPAWN,
			DELAY
		};

		BoardManager* m_boardManager = nullptr;

		GameState m_gameState = GameState::INITIALIZE;
		GameState m_reservedGameState = GameState::NONE;

		float m_delayTimer = 0.0f;
		float m_delayTime = 300.0f;
		
		GridIndex m_firstSelected = { -1, -1 };
		GridIndex m_secondSelected = { -1, -1 };
	};

}


