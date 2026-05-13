#pragma once
#include "Board.h"
#include <random>
#include "Fruit.h"


void Board::InitBoard(const BoardLayout& layout)
{
	// 기존 데이터가 있다면 정리
	if (m_fruitPtrTable) {
		for (int i = 0; i < m_maxRow * m_maxCol; ++i) delete m_fruitPtrTable[i];
		delete[] m_fruitPtrTable;
	}

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

	// 과일 개수만큼 테이블 공간 할당
	m_fruitPtrTable = new Fruit * [m_maxRow * m_maxCol];

	// 랜덤 과일을 위한 준비
	std::random_device rd;
	unsigned long long seed = rd();
	std::mt19937 gen(seed);
	std::uniform_int_distribution<int> dis(0, (int)FruitType::COUNT-1);

	for (int row = 0; row < m_maxCol; row++)
	{
		for (int col = 0; col < m_maxRow; col++)
		{
			Fruit* pNewFruit = new Fruit(ObjectType::FRUIT);

			Point fruitPos;
			fruitPos.x = GetPosition().x + GetGridOffsetX() + (GetCellWidth() / 2) + ( col * (GetCellWidth() + GetGridGap()));
			fruitPos.y = GetPosition().y + GetGridOffsetY() + (GetCellHeight() / 2) + (row * (GetCellHeight() + GetGridGap()));

			pNewFruit->SetPosition(fruitPos.x, fruitPos.y);
			pNewFruit->SetWidth(80);
			pNewFruit->SetWidth(80);
			pNewFruit->SetRow(row);
			pNewFruit->SetCol(col);
			pNewFruit->SetFruitType(static_cast<FruitType>(dis(gen)));
			
			m_fruitPtrTable[(row * m_maxCol) + col] = pNewFruit;
		}
	}
}

int Board::GetBoardWidth() const { return m_boardWidth; }
int Board::GetBoardHeight() const { return m_boardHeight; }
int Board::GetCellWidth() const { return m_cellWidth; }
int Board::GetCellHeight() const { return m_cellHeight; }
int Board::GetMaxRow() const { return m_maxRow; }
int Board::GetMaxCol() const { return m_maxCol; }
int Board::GetGridOffsetX() const { return m_gridOffsetX; }
int Board::GetGridOffsetY() const { return m_gridOffsetY; }
int Board::GetGridGap() const { return m_gridGap; }

// 보드 좌표 -> 블럭 인덱스
// 보드 내의 클릭한 위치의 블럭 인덱스를 얻기
// 유효한 보드좌표면 true, 유효하지않으면 false
bool Board::GetClickedCellIndex(Point boardPos, Index& cellIndex)
{
	// 그리드 범위 밖을 클릭했다면 false
	if ((boardPos.x < m_gridOffsetX || (m_gridOffsetX + m_gridWidth) <= boardPos.x) ||
		(boardPos.y < m_gridOffsetY || (m_gridOffsetY + m_gridHeight) <= boardPos.y))
	{
		return false;
	}

	// 상대 좌표 계산
	int relX = boardPos.x - m_gridOffsetX;
	int relY = boardPos.y - m_gridOffsetY;

	// cell + gap = unit 너비, 높이 계산
	int unitWidth = m_cellWidth + m_gridGap;
	int unitHeight = m_cellHeight + m_gridGap;

	// 상대 좌표 / unityXY ->  셀 인덱스 계산
	int col = relX / unitWidth;
	int row = relY / unitHeight;

	// 유닛 내의 간격을 클릭했는지 검사
	if (relX % unitWidth > m_cellWidth || relY % unitHeight > m_cellHeight)
	{
		return false;
	}

	cellIndex.col = col;
	cellIndex.row = row;
	return true;
}

Fruit* Board::GetFruitAt(int row, int col)
{
	if ((row < 0 || row >= m_maxRow) ||
		(col < 0 || col >= m_maxCol))
	{
		return nullptr;
	}

	return m_fruitPtrTable[(row * m_maxCol) + col];
}