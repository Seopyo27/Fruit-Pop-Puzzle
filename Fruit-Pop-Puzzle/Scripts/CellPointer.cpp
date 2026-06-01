#include "CellPointer.h"
#include "Components/Transform.h"
#include "Components/SpriteRenderer.h"
#include "Scripts/Board.h"
#include "Scripts/GameManager.h"
#include "Types/Types.h"

namespace EHEngine
{
	void CellPointer::Start()
	{
		m_board = m_scene->FindGameObjectByName("Board")->GetComponent<Board>();
		m_gameManager = m_scene->FindGameObjectByName("GameManager")->GetComponent<GameManager>();
		m_currentCellPointer = m_scene->FindGameObjectByName("CurrentCellPointer");
		m_FirstSelectedCellPointer = m_scene->FindGameObjectByName("FirstSelectedCellPointer");
		m_SecondSelectedCellPointer = m_scene->FindGameObjectByName("SecondSelectedCellPointer");
	}

	void CellPointer::Update(float dT)
	{
		GameState state = m_gameManager->GetGameState();
		ScreenPos screenPos;
		BoardIndex index;

		//*************************************************
		// 현재 셀 포인터 On, Off
		//************************************************* 
		if ( (state == GameState::WAITING || state == GameState::SELECTED_FIRST_CELL) &&
			 m_board->GetBoardIndexFromScreenPos(m_scene->GetInputManager()->GetInput().mousePos, index) )
		{
			if (m_board->GetScreenPosFromBoardIndex(index, screenPos))
			{
				m_currentCellPointer->GetComponent<Transform>()->SetPosition(screenPos.ToVector2<learning::Vector2f>());
				m_currentCellPointer->GetComponent<SpriteRenderer>()->SetIsVisible(true);
			}
		}
		else
		{
			m_currentCellPointer->GetComponent<SpriteRenderer>()->SetIsVisible(false);
		}

		
		//*************************************************
		// 첫번째 선택한 셀 포인터 On
		//************************************************* 
		if ( state == GameState::SELECTED_FIRST_CELL &&
			 m_board->GetScreenPosFromBoardIndex(m_gameManager->GetFirstSelected(), screenPos) )
		{
			m_FirstSelectedCellPointer->GetComponent<Transform>()->SetPosition(screenPos.ToVector2<learning::Vector2f>());
			m_FirstSelectedCellPointer->GetComponent<SpriteRenderer>()->SetIsVisible(true);
		}


		//*************************************************
		// 두번째 선택한 셀 포인터 On
		//************************************************* 
		if (state == GameState::SWAP &&
			m_board->GetScreenPosFromBoardIndex(m_gameManager->GetSecondSelected(), screenPos))
		{
			m_SecondSelectedCellPointer->GetComponent<Transform>()->SetPosition(screenPos.ToVector2<learning::Vector2f>());
			m_SecondSelectedCellPointer->GetComponent<SpriteRenderer>()->SetIsVisible(true);
		}
		

		//*************************************************
		// 첫번째, 두번째 선택한 셀 포인터 Off
		//************************************************* 
		if (state == GameState::INITIALIZE || state == GameState::POPANIMATION)
		{
			m_FirstSelectedCellPointer->GetComponent<SpriteRenderer>()->SetIsVisible(false);
			m_SecondSelectedCellPointer->GetComponent<SpriteRenderer>()->SetIsVisible(false);
		}
	}
}
