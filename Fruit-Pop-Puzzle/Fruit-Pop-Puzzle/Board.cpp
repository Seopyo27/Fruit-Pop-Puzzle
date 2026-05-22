#include "Board.h"
#include "Utillity.h"
#include "GameObject.h"
#include <random>
#include <iostream>

namespace EHEngine
{
	int Board::GetRowColToIndex(const GridIndex& index)
	{
		return (index.row * m_maxCol) + index.col;
	}

	learning::Vector2f Board::GetCellCenterPos(const GridIndex& index)
	{
		learning::Vector2f cellCenterPos;

		cellCenterPos.x = m_gridOffsetX + (m_cellWidth / 2) + (index.col * (m_cellWidth + m_gridGap));
		cellCenterPos.y = m_gridOffsetY + (m_cellHeight / 2) + (index.row * (m_cellHeight + m_gridGap));

		return cellCenterPos;
	}

	void Board::InitBoard(const BoardLayout& layout)
	{
		// 레이아웃 설정
		m_boardWidth = layout.boardWidth;
		m_boardHeight = layout.boardHeight;
		m_cellWidth = layout.cellWidth;
		m_cellHeight = layout.cellHeight;
		m_maxRow = layout.maxRow;
		m_maxCol = layout.maxCol;
		m_gridOffsetX = layout.gridOffsetX;
		m_gridOffsetY = layout.gridOffsetY;
		m_gridGap = layout.gridGap;
		m_gridWidth = (m_cellWidth * m_maxCol) + (m_gridGap * (m_maxCol - 1));
		m_gridHeight = (m_cellHeight * m_maxRow) + (m_gridGap * (m_maxRow - 1));

		m_fruitTable.assign(m_maxRow * m_maxCol, Fruit{});
	}

	// 해당 위치에 과일 배치
	void Board::PlaceFruit(const Fruit& fruit, const GridIndex& index)
	{
		m_fruitTable[GetRowColToIndex(index)] = fruit;
	}

	Fruit& Board::GetFruitAt(const GridIndex& index)
	{
		return m_fruitTable[GetRowColToIndex(index)];
	}

	// 과일 데이터 스왑
	void Board::SwapFruit(const GridIndex& i1, const GridIndex& i2)
	{
		Fruit tempFruit = m_fruitTable[GetRowColToIndex(i1)];
		m_fruitTable[GetRowColToIndex(i1)] = m_fruitTable[GetRowColToIndex(i2)];
		m_fruitTable[GetRowColToIndex(i2)] = tempFruit;
	}

	// 과일 데이터 이동
	void Board::MoveFruit(const GridIndex& from, const GridIndex& to)
	{
		// 보드 인덱스 이동, 데이터 덮어쓰기
		m_fruitTable[GetRowColToIndex(to)] = m_fruitTable[GetRowColToIndex(from)];

		// 원래 자리 초기화
		m_fruitTable[GetRowColToIndex(from)] = { FruitType::NONE, nullptr };
	}

	bool  Board::IsAdjacent(const GridIndex& index1, const GridIndex& index2)
	{
		// 인덱스가 보드안에 있는지 확인
		if ((index1.row < 0 || m_maxRow <= index1.row) ||
			(index1.col < 0 || m_maxCol <= index1.col) ||
			(index2.row < 0 || m_maxRow <= index2.row) ||
			(index2.col < 0 || m_maxCol <= index2.col))
		{
			return false;
		}

		// 같은 인덱스 인지 확인
		if ((index1.row == index2.row) && (index1.col == index2.col))
		{
			return false;
		}

		return abs(index1.row - index2.row) + abs(index1.col - index2.col) == 1;
	}

	void Board::DeleteFruit(const GridIndex& index)
	{
		m_fruitTable[GetRowColToIndex(index)].type = FruitType::NONE;
		m_fruitTable[GetRowColToIndex(index)].gameObject = nullptr;
	}

	bool Board::ExistFruit(const GridIndex& index)
	{
		return m_fruitTable[GetRowColToIndex(index)].type != FruitType::NONE &&
			   m_fruitTable[GetRowColToIndex(index)].gameObject != nullptr;
	}
}