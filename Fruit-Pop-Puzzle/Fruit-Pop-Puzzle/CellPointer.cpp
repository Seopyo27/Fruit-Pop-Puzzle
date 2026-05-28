#include "CellPointer.h"
#include "GameObject.h"
#include "GameApp.h"
#include "GameManager.h"
#include "Transform.h"
#include "Sprite.h"

namespace EHEngine
{
	void CellPointer::Start()
	{
		m_gameManager = m_pGame->FindGameObjectByName("Board")->GetComponent<GameManager>();
		m_currentCellPointer = m_pGame->FindGameObjectByName("CurrentCellPointer");
		m_FirstSelectedCellPointer = m_pGame->FindGameObjectByName("FirstSelectedCellPointer");
		m_SecondSelectedCellPointer = m_pGame->FindGameObjectByName("SecondSelectedCellPointer");
	}

	void CellPointer::Update(float dT)
	{
		SCREEN_POS screenPos;

		if (m_gameManager->GetScreenPosCurrentPoint(screenPos) &&
			(m_gameManager->GetGameState() == GameState::WAITING || m_gameManager->GetGameState() == GameState::SELECTED_FIRST_CELL))
		{
			m_currentCellPointer->GetComponent<Transform>()->SetPosition(screenPos.x, screenPos.y - 2);
			m_currentCellPointer->GetComponent<Sprite>()->SetIsVisible(true);
		}
		else
		{
			m_currentCellPointer->GetComponent<Sprite>()->SetIsVisible(false);
		}

		if (m_gameManager->GetGameState() == GameState::SELECTED_FIRST_CELL &&
			m_gameManager->GetScreenPosFirstSelectedPoint(screenPos))
		{
			m_FirstSelectedCellPointer->GetComponent<Transform>()->SetPosition(screenPos.x, screenPos.y - 2);
			m_FirstSelectedCellPointer->GetComponent<Sprite>()->SetIsVisible(true);
		}

		if (m_gameManager->GetGameState() == GameState::SWAP &&
			m_gameManager->GetScreenPosSecondSelectedPoint(screenPos))
		{
			m_SecondSelectedCellPointer->GetComponent<Transform>()->SetPosition(screenPos.x, screenPos.y - 2);
			m_SecondSelectedCellPointer->GetComponent<Sprite>()->SetIsVisible(true);
		}

		if (m_gameManager->GetGameState() == GameState::INITIALIZE ||
			m_gameManager->GetGameState() == GameState::POPANIMATION)
		{
			m_FirstSelectedCellPointer->GetComponent<Sprite>()->SetIsVisible(false);
			m_SecondSelectedCellPointer->GetComponent<Sprite>()->SetIsVisible(false);
		}


	}

}
