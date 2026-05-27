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

		if (m_gameManager->GetScreenPosCurrentPoint(screenPos))
		{
			m_currentCellPointer->GetComponent<Transform>()->SetPosition(screenPos.x, screenPos.y - 2);
			m_currentCellPointer->GetComponent<Sprite>()->SetIsVisible(true);
		}
		else
		{
			m_currentCellPointer->GetComponent<Sprite>()->SetIsVisible(false);
		}
	}

}
