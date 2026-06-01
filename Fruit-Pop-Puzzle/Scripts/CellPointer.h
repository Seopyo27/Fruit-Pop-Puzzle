#pragma once
#include "Components/Script.h"

namespace EHEngine
{
	class GameManager;
	class Board;

	class CellPointer : public Script
	{
	public:
		CellPointer(GameObject* pOwner, Scene* scene) : Script(pOwner, scene) {}

		void Start() override;
		void Update(float dT) override;

	private:
		GameManager* m_gameManager = nullptr;                // 게임 상태 조회용
		Board* m_board = nullptr;                            // 포인트 위치 조회용
		GameObject* m_currentCellPointer = nullptr;          // 현재 셀 포인터
		GameObject* m_FirstSelectedCellPointer = nullptr;    // 첫번째 선택한 셀 포인터
		GameObject* m_SecondSelectedCellPointer = nullptr;   // 두번째 선택한 셀 포인터
	};

}


