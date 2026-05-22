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
			94,
			95,
			6,
			6,
			102,
			93,
			6
		};

		
		m_boardManager = new BoardManager(m_pOwnerObj, m_pGame);
		m_boardManager->InitBoard(layout);
		m_boardManager->RefillAllFruit();
	}

	void GameManager::Update(float dT)
	{
		
		if (m_gameState == GameState::INITIALIZE)
		{
			m_firstSelected = { -1, -1 };
			m_secondSelected = { -1, -1 };

			m_gameState = GameState::WAITING;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << "초기화 완료 턴 진행 시작" << std::endl;
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
					std::cout << "두번째 그리드 선택 완료 : (" << m_firstSelected.row << ", " << m_firstSelected.col << ")" << std::endl;
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
			m_gameState = GameState::INITIALIZE;
			std::cout << "셀 스왑 완료" << std::endl;
		}
	}
}