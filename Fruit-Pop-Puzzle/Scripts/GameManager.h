#pragma once
#include "Components/Script.h"
#include "Scripts/BoardManager.h"
#include "Types/Types.h"
#include "Scripts/ComboEffect.h"
#include <functional>
#include <algorithm>

namespace EHEngine
{
	enum class GameState
	{
		NONE,
		INITIALIZE,             // 초기화
		WAITING,                // 최초 입력 대기
		SELECTED_FIRST_CELL,    // 첫번째 셀 선택 완료
		SWAP,                   // 두번째 셀 선택 완료 및 스왑
		MATCH,                  // 매칭 시도
		POPANIMATION,           // 애니메이션
		POP,                    // 매칭 과일 삭제
		DROP,                   // 과일 드랍
		RESPAWN,                  // 과일 생성

		DELAY                   // [딜레이용]
	};

	class GameManager : public Script
	{
	public:
		GameManager(GameObject* ownerObj, Scene* scene) : Script(ownerObj, scene) {}

		using ScoreChangedCallback = std::function<void(int)>;

		void Start() override
		{
			// 보드정보 로딩
			BoardLayout layout =
			{
				8,
				8,
				573,
				569,
				69,
				69,
				3
			};

			GameObject* board = m_scene->FindGameObjectByName("Board");
			m_boardManager = std::make_unique<BoardManager>();
			m_boardManager->SetBoard(board->GetComponent<Board>());

			m_boardManager->InitBoard(layout);
			m_boardManager->FillBoard();

			m_comboEffect = m_scene->FindGameObjectByName("ComboEffectManager")->GetComponent<ComboEffect>();
		}

		void Update(float dT) override
		{
			//*************************************************
			// 딜레이
			//************************************************* 
			if (m_gameState == GameState::DELAY)
			{
				m_delayTimer += dT;
				if (m_delayTimer < m_delayTime) return;
				m_delayTimer = 0;

				m_gameState = m_reservedGameState;
			}


			//*************************************************
			// 초기화
			//************************************************* 
			else if (m_gameState == GameState::INITIALIZE)
			{
				m_firstSelected = { -1, -1 };
				m_secondSelected = { -1, -1 };
				m_comboCount = 0;

				m_gameState = GameState::WAITING;
			}


			//*************************************************
			// 첫번째 과일 클릭 대기
			//************************************************* 
			else if (m_gameState == GameState::WAITING)
			{
				if (m_scene->GetInputManager()->GetInput().key != KeyState::MOUSE_LEFT_CLICK) return;

				MousePos mousePos = m_scene->GetInputManager()->GetInput().mousePos;
				BoardIndex selectedIndex;

				if (false == m_boardManager->GetBoardIndexFromScreenPos(mousePos, selectedIndex)) return;

				m_firstSelected = selectedIndex;

				m_gameState = GameState::SELECTED_FIRST_CELL;
			}


			//*************************************************
			// 두번째 과일 클릭 대기
			//************************************************* 
			else if (m_gameState == GameState::SELECTED_FIRST_CELL)
			{
				if (m_scene->GetInputManager()->GetInput().key != KeyState::MOUSE_LEFT_CLICK) return;

				MousePos mousePos = m_scene->GetInputManager()->GetInput().mousePos;
				BoardIndex selectedIndex;

				if (false == m_boardManager->GetBoardIndexFromScreenPos(mousePos, selectedIndex)) return;

				m_secondSelected = selectedIndex;

				if (false == m_boardManager->IsAdjacent(m_firstSelected, m_secondSelected))
				{
					m_gameState = GameState::INITIALIZE;
					return;
				}
				
				m_gameState = GameState::SWAP;
			}


			//*************************************************
			// 과일 스왑
			//************************************************* 
			else if (m_gameState == GameState::SWAP)
			{
				m_boardManager->SwapFruit(m_firstSelected, m_secondSelected);

				m_gameState = GameState::DELAY;
				m_reservedGameState = GameState::MATCH;
			}


			//*************************************************
			// 과일 매칭
			//************************************************* 
			else if (m_gameState == GameState::MATCH)
			{
				m_boardManager->FindMatches();

				if (m_boardManager->ExistMatchedFruit())   // 매칭된 과일 있음
				{
					m_gameState = GameState::DELAY;
					m_reservedGameState = GameState::POPANIMATION;
					return;
				}

				if (m_comboCount >= 1)   // 콤보 중에는 과일 재스왑 X
				{
					m_gameState = GameState::INITIALIZE;
					return;
				}

				m_boardManager->SwapFruit(m_secondSelected, m_firstSelected);   // 콤보 중이 아닐 때는 재스왑
				m_gameState = GameState::DELAY;
				m_reservedGameState = GameState::INITIALIZE;
			}


			//*************************************************
			// 과일 터지는 애니메이션 실행
			//************************************************* 
			else if (m_gameState == GameState::POPANIMATION)
			{
				m_comboCount += 1;

				m_boardManager->PlayPopAnimation();

				if (m_comboCount == 2)
				{
					m_comboEffect->ShowComboEffect("Monkey", { 120, 180 }, { 180, 180 }, 1000.0f, "Good");
				}
				else if (m_comboCount == 3)
				{
					m_comboEffect->ShowComboEffect("Chameleon", { 700, 280 }, { 630, 280 }, 1000.0f, "Great");
				}
				else if (m_comboCount == 4)
				{
					m_comboEffect->ShowComboEffect("Parrot", { 130, 700 } , { 200, 660 }, 1000.0f, "Awesome");
				}
				else if (m_comboCount == 5)
				{
					m_comboEffect->ShowComboEffect("Hippo", { 610, 720 }, { 610, 660 }, 1000.0f, "Fantastic");
				}
				else if (m_comboCount >= 6)
				{
					m_comboEffect->ShowComboEffect("Animals", { 400, 700 }, { 400, 660 }, 1000.0f, "Fever");
				}

				m_gameState = GameState::DELAY;
				m_reservedGameState = GameState::POP;
			}


			//*************************************************
			// 과일 제거
			//************************************************* 
			else if (m_gameState == GameState::POP)
			{
				AddScore(m_boardManager->GetMatchedFruitCount() * 100 * (m_comboCount * m_comboCount)); 
				m_boardManager->DeleteMatchedFruit();
				m_boardManager->InitFruitMatchedList();
				m_gameState = GameState::DELAY;
				m_reservedGameState = GameState::DROP;
			}


			//*************************************************
			// 과일 드랍
			//************************************************* 
			else if (m_gameState == GameState::DROP)
			{
				m_boardManager->ProcessFruitDrop();
				m_gameState = GameState::DELAY;
				m_reservedGameState = GameState::RESPAWN;
			}


			//*************************************************
			// 과일 생성
			//************************************************* 
			else if (m_gameState == GameState::RESPAWN)
			{
				m_boardManager->FillEmptySpaces();
				m_gameState = GameState::MATCH;
			}

		}

		void AddScore(int amount)
		{
			m_score = (std::min)(m_score + amount, 999999999);
			NotifyScoreChanged();
		}

		void BindScoreChanged(ScoreChangedCallback callback)
		{
			scoreChangedCallbacks.push_back(callback);
		}
	
		GameState GetGameState() { return m_gameState; }

		const BoardIndex& GetFirstSelected() const { return m_firstSelected; }
		const BoardIndex& GetSecondSelected() const { return m_secondSelected; }

	private:
		GameState m_gameState = GameState::INITIALIZE;
		GameState m_reservedGameState = GameState::NONE;
		
		std::unique_ptr<BoardManager> m_boardManager;

		int m_comboCount = 0;

		float m_delayTimer = 0.0f;
		float m_delayTime = 300.0f;

		BoardIndex m_firstSelected = { -1, -1 };
		BoardIndex m_secondSelected = { -1, -1 };

		//*************************************************
		// 점수 표시 이벤트
		//************************************************* 
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

		//*************************************************
		// 콤보 이펙트
		//************************************************* 
		ComboEffect* m_comboEffect = nullptr;

	};
}


