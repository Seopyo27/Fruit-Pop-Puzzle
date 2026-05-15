#pragma once
#include "Script.h"

class Transform;
class Board;

class BoardManager : public Script
{
public:
	BoardManager(NewGameObject* pOwner) : Script(pOwner) {}
	~BoardManager() override;

	void Start() override;
	void Update(float dT) override;
	void FixedUpdate() override;
	void End() override;

private:
	int m_boardWidth; //  보드 이미지 너비
	int m_boardHeight; // 보드 이미지 높이
	int m_cellWidth; // 그리드 너비
	int m_cellHeight; // 그리드 높이
	int m_maxRow; // 행 개수
	int m_maxCol; // 열개수
	int m_gridOffsetX; // 그리드 시작 X 좌표
	int m_gridOffsetY; // 그리드 시작 Y 좌표
	int m_gridGap; // 그리드 간격

	Transform* m_transform;
	Board* m_board;
};