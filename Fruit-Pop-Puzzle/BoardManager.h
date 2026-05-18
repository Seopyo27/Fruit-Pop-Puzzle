#pragma once
#include "Script.h"
#include <vector>
#include <set>
#include "Fruit.h"

class Transform;
class Board;

class BoardManager : public Script
{
public:
	BoardManager(NewGameObject* pOwner, MyFirstWndGame* game) : Script(pOwner, game) {}
	~BoardManager() override;

	void Start() override;
	void Update(float dT) override;
	void FixedUpdate() override;
	void End() override;

private:
	int m_boardWidth = 800; //  보드 이미지 너비
	int m_boardHeight = 800; // 보드 이미지 높이
	int m_cellWidth= 94; // 그리드 너비
	int m_cellHeight = 95; // 그리드 높이
	int m_maxRow = 6; // 행 개수
	int m_maxCol = 6; // 열개수
	int m_gridOffsetX = 102; // 그리드 시작 X 좌표
	int m_gridOffsetY = 93; // 그리드 시작 Y 좌표
	int m_gridGap = 6; // 그리드 간격

	std::vector<Fruit*> m_fruitPtrTable;
	std::set<Index> m_fruitMatchedList;

	Transform* m_transform;
	Board* m_board;

private:
	void SpawnFruit(const Index& index);
	int GetRowColToIndex(int row, int col);
	Pos GetCellCenterPos(Index cellIndex);
};