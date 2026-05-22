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

		void CreateFruit(const GridIndex& index);
		void DeleteFruit(const GridIndex& index);
		void PlaceFruit(const Fruit& fruit, const GridIndex& index);
		void MoveFruit(const GridIndex& from, const GridIndex& to);
		void RefillAllFruit();
		void FillFruitEmptySpaces();

		void DropFruits();

		void SwapFruit(const GridIndex& index1, const GridIndex& index2);
		bool IsAdjacent(const GridIndex& index1, const GridIndex& index2);

		void FindMathes();
		void FindCrossMatches(const GridIndex& startIndex);
		void FindBoxMatches(const GridIndex& startIndex);

		void InitFruitMatchedList();
		bool ExistMatchedFruit();
		void DeleteMatchedFruit();

		bool GetGridIndexFromScreenPos(const MOUSE_POS& mousePos, GridIndex& gridIndex);
		bool GetPosScreenToBoard(const MOUSE_POS& mousePos, MOUSE_POS& boardPos);
		bool GetGridIndexFromBoardPos(const MOUSE_POS& boardPos, GridIndex& gridIndex);

		void PrintBoard();
		void PrintMatchedList();
		void PrintFruitBitmap(const GridIndex& index);

	private:
		Board* m_board = nullptr;
		std::set<GridIndex> m_fruitMatchedList;
	};
}


