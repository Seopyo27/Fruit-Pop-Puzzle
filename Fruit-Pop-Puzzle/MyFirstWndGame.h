#pragma once

#include "NzWndBase.h"
#include "Utillity.h"
#include "Board.h"

// [CHECK] #7  전방 선언을 사용하여 헤더파일의 의존성을 줄임.
class GameTimer;
class GameObjectBase;
class GameObject;
class Board;

namespace renderHelp
{
	class BitmapInfo;
}

class MyFirstWndGame : public NzWndBase
{
public:
	MyFirstWndGame() = default;
	~MyFirstWndGame() override = default;

	bool Initialize();
	void Run();
	void Finalize();

private:
	struct MOUSE_POS
	{
		int x = 0;
		int y = 0;

		bool operator!=(const MOUSE_POS& other) const
		{
			return (x != other.x || y != other.y);
		}
	};

	void Update();
	void Render();

	void OnResize(int width, int height) override;
	void OnClose() override;

	void OnMouseMove(int x, int y);
	void OnLButtonDown(int x, int y);
	void OnRButtonDown(int x, int y);

	void FixedUpdate();
	void LogicUpdate();

	void CreateBoard(int boardWidth, int boardHeight, int cellWidth, int cellHeight, int maxRow, int maxCol, int gridOffsetX, int gridOffsetY, int gridGap);

	void CreatePlayer();
	void CreateEnemy();

	void UpdatePlayerInfo();

	bool ConvertScreenToBoard(int mouseX, int mouseY, Point& boardPos);

	GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }

private:
	HDC m_hFrontDC = nullptr;
	HDC m_hBackDC = nullptr;
	HBITMAP m_hBackBitmap = nullptr;
	HBITMAP m_hDefaultBitmap = nullptr;

	// [CHECK] #8. 게임 타이머를 사용하여 프레임을 관리하는 예시.
	GameTimer* m_pGameTimer = nullptr;

	float m_fDeltaTime = 0.0f;
	float m_fFrameCount = 0.0f;

	// [CHECK] #8. 게임 오브젝트를 관리하는 컨테이너.
	GameObjectBase** m_GameObjectPtrTable = nullptr;
	int m_GameObjectCount = 0;

	MOUSE_POS m_MousePos = { 0, 0 };
	MOUSE_POS m_MousePosPrev = { 0, 0 };

	MOUSE_POS m_PlayerTargetPos = { 0, 0 };
	MOUSE_POS m_EnemySpawnPos = { 0, 0 };

	using BitmapInfo = renderHelp::BitmapInfo;

	BitmapInfo* m_pBoardBitmapInfo = nullptr;

	// 퍼즐 게임 추가
	Board* m_pBoard = nullptr;
};