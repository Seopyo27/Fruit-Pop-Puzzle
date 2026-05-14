#pragma once
#include "Board.h"
#include <random>
#include "Fruit.h"
#include <cmath>

void Board::Render(HDC hdc)
{
	GameObject::Render(hdc);

	if (m_fruitPtrTable == nullptr) return;

	for (int i = 0; i < m_maxCol * m_maxRow; i++)
	{
		Fruit* pFruit = m_fruitPtrTable[i];
		if (pFruit == nullptr) continue;
		pFruit->Render(hdc);
	}
}

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

	// 행 * 열 만큼 과일 테이블 공간 할당
	m_fruitPtrTable = new Fruit * [m_maxRow * m_maxCol];
}

void Board::SetPFruitBitmapInfoTable(renderHelp::BitmapInfo** pFruitBitmapInfoTable)
{
	m_pFruitBitmapInfoTable = pFruitBitmapInfoTable;
}

void Board::InitAllFruit()
{
	// 랜덤 과일을 위한 준비
	std::random_device rd;
	unsigned long long seed = rd();
	std::mt19937 gen(seed);
	std::uniform_int_distribution<int> dis(0, (int)FruitType::COUNT - 1);

	for (int row = 0; row < m_maxRow; row++)
	{
		for (int col = 0; col < m_maxCol; col++)
		{
			Fruit* pNewFruit = new Fruit();

			Pos fruitPos;

			Pos cellCenterPos = GetCellCenterPos({ row, col });

			pNewFruit->SetPosition((m_pos.x - m_width / 2) + cellCenterPos.x, (m_pos.y - m_height / 2) + cellCenterPos.y);
			pNewFruit->SetWidth(80);
			pNewFruit->SetHeight(80);
			pNewFruit->SetRow(row);
			pNewFruit->SetCol(col);
			pNewFruit->SetFruitType(static_cast<FruitType>(dis(gen)));

			if (m_pFruitBitmapInfoTable != nullptr)
			{
				pNewFruit->SetBitmapInfo(m_pFruitBitmapInfoTable[int(pNewFruit->GetFruitType())]);
			}
			
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
bool Board::GetClickedCellIndex(Pos boardPos, Index& cellIndex)
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

Fruit* Board::GetFruitAt(Index index)
{
	if ((index.row < 0 || index.row >= m_maxRow) ||
		(index.col < 0 || index.col >= m_maxCol))
	{
		return nullptr;
	}

	return m_fruitPtrTable[GetRowColToIndex(index.row, index.col)];
}

int Board::GetRowColToIndex(int row, int col)
{
	return (row * m_maxCol) + col;
}

Pos Board::GetCellCenterPos(Index cellIndex)
{
	Pos cellCenterPos;

	cellCenterPos.x = m_gridOffsetX + (GetCellWidth() / 2) + (cellIndex.col * (GetCellWidth() + GetGridGap()));
	cellCenterPos.y = m_gridOffsetY + (GetCellHeight() / 2) + (cellIndex.row * (GetCellHeight() + GetGridGap()));

	return cellCenterPos;
}

void Board::SwapFruit(const Index& f1, const Index& f2)
{
	int index1 = GetRowColToIndex(f1.row, f1.col);
	int index2 = GetRowColToIndex(f2.row, f2.col);
	Fruit* pFruit1 = m_fruitPtrTable[index1];
	Fruit* pFruit2 = m_fruitPtrTable[index2];

	// 위치 스왑
	float tempPosX, tempPosY;
	tempPosX = pFruit1->GetPosition().x;
	tempPosY = pFruit1->GetPosition().y;

	pFruit1->SetPosition(pFruit2->GetPosition().x, pFruit2->GetPosition().y);
	pFruit2->SetPosition(tempPosX, tempPosY);

	// 과일 인스턴스 맴버 인덱스 스왑
	Index tempIndex;

	tempIndex.row = pFruit1->GetRow();
	tempIndex.col = pFruit1->GetCol();

	pFruit1->SetRow(pFruit2->GetRow());
	pFruit1->SetCol(pFruit2->GetCol());
	pFruit2->SetRow(tempIndex.row);
	pFruit2->SetCol(tempIndex.col);

	// 보드 과일 테이블 위치 스왑
	Fruit* pTemp = pFruit1;
	m_fruitPtrTable[index1] = pFruit2;
	m_fruitPtrTable[index2] = pTemp;
}

bool Board::IsAdjacent(const Index& index1, const Index& index2)
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
