#pragma once
#include "Script.h"
#include "Types.h"

namespace EHEngine
{
	class GameManager;
	class GameObject;
	class Transform;
	class Sprite;

	class CellPointer : public Script
	{
	public:
		CellPointer(GameObject* pOwner, GameApp* game) : Script(pOwner, game) {}

		void Start() override;
		void Update(float dT) override;


	private:
		// 어디에 포인터를 찍어야하는지 정보를 가져올 게임 매니저
		GameManager* m_gameManager = nullptr;

		// 현재 셀 포인터
		GameObject* m_currentCellPointer = nullptr;

		// 첫번째 선택한 셀 포인터
		GameObject* m_FirstSelectedCellPointer = nullptr;

		// 두번째 선택한 셀 포인터
		GameObject* m_SecondSelectedCellPointer = nullptr;


	};
}

