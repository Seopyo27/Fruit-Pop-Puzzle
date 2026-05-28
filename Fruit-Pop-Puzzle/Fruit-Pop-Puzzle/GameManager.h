#pragma once
#include "Script.h"
#include "Board.h"
#include <functional>
#include "Types.h"

namespace EHEngine
{
	class BoardManager;

	enum class GameState
	{
		NONE,
		INITIALIZE,
		WAITING,
		SELECTED_FIRST_CELL,
		SWAP,
		MATCH,
		POPANIMATION,
		POP,
		DROP,
		SPAWN,
		DELAY
	};

	class GameManager : public Script
	{
	public:
		// 델리게이터를 만들기 위한 데이터 형 선언
		using ScoreChangedCallback = std::function<void(int)>;

		GameManager(GameObject* pOwner, GameApp* game) : Script(pOwner, game) {}

		void Start() override;
		void Update(float dT) override;
		void AddScore(int amount);
		void BindScoreChanged(ScoreChangedCallback callback)
		{
			scoreChangedCallbacks.push_back(callback);
		}
		
		bool GetScreenPosCurrentPoint(SCREEN_POS& screenPos);
		bool GetScreenPosFirstSelectedPoint(SCREEN_POS& screenPos);
		bool GetScreenPosSecondSelectedPoint(SCREEN_POS& screenPos);


		GameState GetGameState() { return m_gameState; }
	private:
		BoardManager* m_boardManager = nullptr;

		GameState m_gameState = GameState::INITIALIZE;
		GameState m_reservedGameState = GameState::NONE;

		float m_delayTimer = 0.0f;
		float m_delayTime = 300.0f;
		
		GridIndex m_firstSelected = { -1, -1 };
		GridIndex m_secondSelected = { -1, -1 };

		// 점수
		int m_score = 0;

		// 델리게이터
		std::vector<ScoreChangedCallback> scoreChangedCallbacks;

		// 델리게이터 순회, 콜백
		void NotifyScoreChanged()
		{
			for (auto& callback : scoreChangedCallbacks)
			{
				callback(m_score);
			}
		}

		bool m_isCombo = false;


	};

}


