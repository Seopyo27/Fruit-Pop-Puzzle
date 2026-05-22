#pragma once
#include "Script.h"
#include "Board.h"
#include "Types.h"

namespace EHEngine
{

	class BoardManager : public Script
	{
	public:
		BoardManager(GameObject* pOwner, GameApp* game) : Script(pOwner, game) {}

		void InitBoard(const BoardLayout& layout);

		void SpawnFruit(const GridIndex& index);
		void RefillAllFruit();

		void SwapFruit(const GridIndex& index1, const GridIndex& index2);
		bool IsAdjacent(const GridIndex& index1, const GridIndex& index2);

		bool GetGridIndexFromScreenPos(const MOUSE_POS& mousePos, GridIndex& gridIndex);
		bool GetPosScreenToBoard(const MOUSE_POS& mousePos, MOUSE_POS& boardPos);
		bool GetGridIndexFromBoardPos(const MOUSE_POS& boardPos, GridIndex& gridIndex);

	private:
		Board* m_board = nullptr;
		
	};
}


