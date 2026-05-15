#pragma once
#include "GameObject.h"
#include "Fruit.h"
#include <vector>
#include <set>

struct Pos
{
	int x, y;
};

struct Index {
	int row, col;

	bool operator<(const Index& other) const
	{
		if (row != other.row)
			return row < other.row;
		return col < other.col;
	}

	Index operator+(const Index& other) const
	{
		return { row + other.row, col + other.col };
	}
};

struct BoardLayout
{
	int boardWidth; //  전체 이미지 너비
	int boardHeight; // 전체 이미지 높이
    int cellWidth; // 그리드 너비
	int cellHeight; // 그리드 높이
	int maxRow; // 행 개수
	int maxCol; // 열개수
	int gridOffsetX; // 그리드 시작 X 좌표
	int gridOffsetY; // 그리드 시작 Y 좌표
	int gridGap; // 그리드 간격
};

class Board : public GameObject
{
public:
	Board(ObjectType type) : GameObject(type) {}
	~Board() override = default;

	void Render(HDC hdc) override;

	void InitBoard(const BoardLayout& layout);
	void SetPFruitBitmapInfoTable(renderHelp::BitmapInfo** pFruitBitmapInfoTable);
	void RefillAllFruit();
	void InitFruitMatchedList();

	void SwapFruit(const Index& f1, const Index& f2);

	int GetBoardWidth() const;
	int GetBoardHeight() const;
	int GetCellWidth() const;
	int GetCellHeight() const;
	int GetMaxRow() const;
	int GetMaxCol() const;
	int GetGridOffsetX() const;
	int GetGridOffsetY() const;
	int GetGridGap() const;

	bool IsAdjacent(const Index& index1, const Index& index2);
	bool ExistMatchedFruit();

	void FindMathes();

	void FillFruitEmptySpaces();

	void SpawnFruit(const Index& index);

	void FIndCrossMatches(const Index& startIndex);
	void FIndBoxMatches(const Index& startIndex);
	void DeleteMatchedFruit();

	Pos GetCellCenterPos(Index cellIndex);
	bool GetClickedCellIndex(Pos boardPos, Index& cellIndex);
	Fruit* GetFruitAt(Index index);
	int GetRowColToIndex(int row, int col);

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

	Fruit** m_fruitPtrTable = nullptr;
	std::set<Index> m_fruitMatchedList;
 	renderHelp::BitmapInfo** m_pFruitBitmapInfoTable = nullptr;
	
};