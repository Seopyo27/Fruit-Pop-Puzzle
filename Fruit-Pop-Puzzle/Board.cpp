#pragma once
#include "Board.h"


void Board::InitBoard(const BoardLayout& layout)
{
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
bool Board::GetClickedCellIndex(Point clickPoint, Index& cellIndex)
{
	// 그리드 범위 밖을 클릭했다면 false
	if ((clickPoint.x < m_gridOffsetX || (m_gridOffsetX + m_gridWidth) <= clickPoint.x) ||
		(clickPoint.y < m_gridOffsetY || (m_gridOffsetY + m_gridHeight) <= clickPoint.y))
	{
		return false;
	}

	// 상대 좌표 계산
	int relX = clickPoint.x - m_gridOffsetX;
	int relY = clickPoint.y - m_gridOffsetY;

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
