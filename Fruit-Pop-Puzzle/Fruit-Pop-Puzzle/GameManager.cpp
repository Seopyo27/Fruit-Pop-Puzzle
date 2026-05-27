#include "GameManager.h"
#include "BoardManager.h"
#include "GameApp.h"

namespace EHEngine
{
	void GameManager::Start()
	{
		// 보드정보 로딩
		BoardLayout layout =
		{
			800,
			800,
			69,
			69,
			8,
			8,
			115,
			119,
			3
		};

		
		m_boardManager = new BoardManager(m_pOwnerObj, m_pGame);
		m_boardManager->InitBoard(layout);
		m_boardManager->RefillAllFruit();
	}

	void GameManager::Update(float dT)
	{
		if (m_gameState == GameState::DELAY)
		{
			m_delayTimer += dT;
			if (m_delayTimer >= m_delayTime)
			{
				m_delayTimer = 0;
				m_gameState = m_reservedGameState;
			}
		}
		
		else if (m_gameState == GameState::INITIALIZE)
		{
			m_firstSelected = { -1, -1 };
			m_secondSelected = { -1, -1 };

			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << "초기화 완료 턴 진행 시작" << std::endl;

			
			m_gameState = GameState::WAITING;
			//char c;
			//std::cin >> c;
			//if (c == 'n')
			//{
			//	int x, y;
			//	std::cin >> x >> y;
			//	m_boardManager->PrintFruitBitmap({ y, x });
			//}

			//else
			//{
			//	
			//}
		}

		else if (m_gameState == GameState::WAITING)
		{
			if (m_pGame->GetInput().key == Key::MOUSE_LEFT_CLICK)
			{
				GridIndex index;
				if (m_boardManager->GetGridIndexFromScreenPos(m_pGame->GetInput().mousePos, index))
				{
					m_firstSelected = index;
					m_gameState = GameState::SELECTED_FIRST_CELL;
					std::cout << "첫번째 그리드 선택 완료 : (" << m_firstSelected.row << ", " << m_firstSelected.col << ")" << std::endl;
				}

				else
				{
					std::cout << "선택 실패 다시 선택해주세요." << std::endl;
				}
			}
		}

		else if (m_gameState == GameState::SELECTED_FIRST_CELL)
		{
			if (m_pGame->GetInput().key == Key::MOUSE_LEFT_CLICK)
			{
				GridIndex index;
				if (m_boardManager->GetGridIndexFromScreenPos(m_pGame->GetInput().mousePos, index))
				{
					m_secondSelected = index;
					std::cout << "두번째 그리드 선택 완료 : (" << m_secondSelected.row << ", " << m_secondSelected.col << ")" << std::endl;
					if (m_boardManager->IsAdjacent(m_firstSelected, m_secondSelected))
					{
						m_gameState = GameState::SWAP;
					}
					else
					{
						m_gameState = GameState::INITIALIZE;
						std::cout << "첫번째와 두번째 그리드가 인접한 그리드가 아닙니다." << std::endl;
					}
				}

				else
				{
					std::cout << "선택 실패 다시 선택해주세요." << std::endl;
				}
			}
		}

		else if (m_gameState == GameState::SWAP)
		{
			m_boardManager->SwapFruit(m_firstSelected, m_secondSelected);
			std::cout << "셀 스왑 완료" << std::endl;
			m_gameState = GameState::MATCH;
		}

		else if (m_gameState == GameState::MATCH)
		{
			m_boardManager->FindMathes();

			if (m_boardManager->ExistMatchedFruit())
			{
				std::cout << "매칭된 과일 있음" << std::endl;
				m_gameState = GameState::DELAY;
				m_reservedGameState = GameState::POPANIMATION;
			}

			else
			{
				std::cout << "매칭된 과일 없음" << std::endl;
				//m_boardManager->SwapFruit(m_secondSelected, m_firstSelected);
				//std::cout << "과일 재스왑 완료" << std::endl;
				m_gameState = GameState::INITIALIZE;
			}
		}

		else if (m_gameState == GameState::POPANIMATION)
		{
			m_boardManager->PlayAnimation();
			m_gameState = GameState::DELAY;
			m_reservedGameState = GameState::POP;
			std::cout << "애니메이션 실행" << std::endl;
		}

		else if (m_gameState == GameState::POP)
		{
			// 점수 카운팅
			AddScore(m_boardManager->GetMatchedFruitCount());
			std::cout << "점수 합산 완료." << std::endl;

			m_boardManager->DeleteMatchedFruit();
			std::cout << "과일 팝 완료." << std::endl;
			m_boardManager->InitFruitMatchedList();
			std::cout << "매칭 벡터 초기화 완료" << std::endl;
			
			m_gameState = GameState::DROP;
		}

		else if (m_gameState == GameState::DROP)
		{
			m_boardManager->CheckDropFruit();
			std::cout << "과일 드랍 완료" << std::endl;
			m_gameState = GameState::DELAY;
			m_reservedGameState = GameState::SPAWN;
		}

		else if (m_gameState == GameState::SPAWN)
		{
			m_boardManager->FillFruitEmptySpaces();
			std::cout << "빈 공간 과일 재 생성 완료" << std::endl;
			m_gameState = GameState::MATCH;
			std::cout << std::endl;
			std::cout << std::endl;
			m_boardManager->PrintBoard();
		}

	}
	
	void GameManager::AddScore(int amount)
	{
		m_score = min(m_score + (m_boardManager->GetMatchedFruitCount() * 100), 999999999);
		NotifyScoreChanged();
	}
}