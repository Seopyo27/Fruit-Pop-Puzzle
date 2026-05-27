#include "CellPointer.h"
#include "GameObject.h"
#include "GameApp.h"
#include "GameManager.h"

namespace EHEngine
{
	void CellPointer::Start()
	{
		GameManager* m_gameManager = m_pGame->FindGameObjectByName("Board")->GetComponent<GameManager>();
		GameObject* m_currentCellPointer = m_pGame->FindGameObjectByName("CurrentCellPointer");
		GameObject* m_FirstSelectedCellPointer = m_pGame->FindGameObjectByName("FirstSelectedCellPointer");
		GameObject* m_SecondSelectedCellPointer = m_pGame->FindGameObjectByName("SecondSelectedCellPointer");
	}

	void CellPointer::Update(float dT)
	{

	}

}
