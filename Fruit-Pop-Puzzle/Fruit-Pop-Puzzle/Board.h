#pragma once
#include <set>
#include <vector>
#include "Utillity.h"
#include "Types.h"

namespace EHEngine
{
	class GameObject;

	enum class FruitType {
		NONE = -1,
		APPLE, // 0
		BANANA, // 1
		GRAPES, // 2
		WATERMELON, // 3
		COUNT // 과일 종류의 개수를 파악하기 위한 용도
	};

	struct Fruit {
		FruitType type = FruitType::NONE;
		GameObject* gameObject = nullptr;
	};

	struct GridIndex
	{
		int row, col;

		bool operator<(const GridIndex& other) const
		{
			if (row != other.row)
				return row < other.row;
			return col < other.col;
		}

		GridIndex operator+(const GridIndex& other) const
		{
			return { row + other.row, col + other.col };
		}
	};

	class Board
	{
	public:
		void InitBoard(const BoardLayout& layout);

		Fruit& GetFruitAt(const GridIndex& index);

		void PlaceFruit(const Fruit& fruit, const GridIndex& index);
		void SwapFruit(const GridIndex& f1, const GridIndex& f2);
		void MoveFruit(const GridIndex& from, const GridIndex& to);
		void DeleteFruit(const GridIndex& index);
		bool ExistFruit(const GridIndex& index);
		
		
		bool IsAdjacent(const GridIndex& index1, const GridIndex& index2);

		void DeleteMatchedFruit();

		int GetRowColToIndex(const GridIndex& index);
		learning::Vector2f GetCellCenterPos(const GridIndex& index);
		std::vector<Fruit>& GetFruitTable() { return m_fruitTable; }
		int GetBoardWidth() const { return m_boardWidth; }
		int GetBoardHeight() const { return m_boardHeight; }
		int GetCellWidth() const { return m_cellWidth; }
		int GetCellHeight() const { return m_cellHeight; }
		int GetMaxRow() const { return m_maxRow; }
		int GetMaxCol() const { return m_maxCol; }
		int GetGridOffsetX() const { return m_gridOffsetX; }
		int GetGridOffsetY() const { return m_gridOffsetY; }
		int GetGridWidth() const { return m_gridWidth; }
		int GetGridHeight() const { return m_gridHeight; }
		int GetGridGap() const { return m_gridGap; }

	private:

	private:
		int	m_boardWidth = 0; // 전체 이미지 너비
		int m_boardHeight = 0; // 전체 이미지 높이
		int m_cellWidth = 0; // 셀 너비
		int m_cellHeight = 0; // 셀 높이
		int m_maxRow = 0; // 행 개수
		int m_maxCol = 0; // 열개수
		int m_gridOffsetX = 0; // 그리드 첫 시작 X 좌표
		int m_gridOffsetY = 0; // 그리드 첫 시작 Y 좌표
		int m_gridGap = 0; // 그리드 간격
		int m_gridWidth = 0; // 그리드 너비
		int m_gridHeight; // 그리드 높이

		std::vector<Fruit> m_fruitTable;
	};

}

