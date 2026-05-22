#include "BoardManager.h"
#include "GameApp.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "GameApp.h"
#include <random>

namespace EHEngine
{
	void BoardManager::InitBoard(const BoardLayout& layout)
	{
		if (m_board != nullptr)
		{
			delete m_board;
		}

		m_board = new Board();
		m_board->InitBoard(layout);
	}

	void BoardManager::SpawnFruit(const GridIndex& index)
	{
		// 랜덤 과일 타입 생성
		static std::random_device rd;
		static unsigned long long seed = rd();
		static std::mt19937 gen(seed);
		static std::uniform_int_distribution<int> dis(0, (int)FruitType::COUNT - 1);

		int fruitTypeNum = dis(gen);
		FruitType fruitType = static_cast<FruitType>(fruitTypeNum);

		// 과일 오브젝트 생성
		GameObject* newObj = m_pGame->CreateGameObject(); // 아 망했다. 키를 이름으로하니까 이런문제가.
		Transform* transform = newObj->GetComponent<Transform>();

		learning::Vector2f cellCenterPos = m_board->GetCellCenterPos(index);
		transform->SetPosition( (m_pOwnerObj->GetComponent<Transform>()->GetPosition().x - m_board->GetBoardWidth() / 2) + cellCenterPos.x,
			                    (m_pOwnerObj->GetComponent<Transform>()->GetPosition().y - m_board->GetBoardHeight() / 2) + cellCenterPos.y );
		transform->SetWidth(80);
		transform->SetHeight(80);

		static const char* fruitNames[] = { "Apple", "Banana", "Grapes" };

		Sprite* sprite = newObj->AddComponent<Sprite>();
		sprite->SetBitmapInfo(m_pGame->GetBitmapInfo(fruitNames[fruitTypeNum]));
		
		// 과일 구조체 세팅
		Fruit newFruit = { fruitType , newObj };

		// 보드에 과일 데이터 추가
		m_board->PlaceFruit(newFruit, index);
	}

	void BoardManager::RefillAllFruit()
	{
		for (int row = 0; row < m_board->GetMaxRow(); row++)
		{
			for (int col = 0; col < m_board->GetMaxCol(); col++)
			{
				SpawnFruit({ row, col });
			}
		}
	}

	void BoardManager::SwapFruit(const GridIndex& index1, const GridIndex& index2)
	{
		m_board->SwapFruit(index1, index2);
	}

	bool BoardManager::IsAdjacent(const GridIndex& index1, const GridIndex& index2)
	{
		return m_board->IsAdjacent(index1, index2);
	}


	bool BoardManager::GetGridIndexFromScreenPos(const MOUSE_POS& mousePos, GridIndex& gridIndex)
	{
		// 스크린 좌표 -> 보드 좌표
		MOUSE_POS boardPos;
		if (!GetPosScreenToBoard(mousePos, boardPos))
		{
			// 보드 이미지 밖
			std::cout << "보드 이미지 밖 입니다." << std::endl;
			return false;
		}

		// 보드 좌표 -> 셀 인덱스
		if (!GetGridIndexFromBoardPos(boardPos, gridIndex))
		{
			// 보드 이미지 안이지만 셀 범위 밖
			std::cout << "보드 이미지 안이지만 그리드 범위 밖 입니다." << std::endl;
			return false;
		}

		return true;
	}

	bool BoardManager::GetPosScreenToBoard(const MOUSE_POS& mousePos, MOUSE_POS& boardPos)
	{
		// 화면에서 보드 시작점 계산
		int boardStartX = m_pOwnerObj->GetComponent<Transform>()->GetPosition().x - (m_board->GetBoardWidth() / 2);
		int boardStartY = m_pOwnerObj->GetComponent<Transform>()->GetPosition().y - (m_board->GetBoardHeight() / 2);

		// 보드를 클릭했는가?
		if ((mousePos.x < boardStartX || boardStartX + m_board->GetBoardWidth() < mousePos.x) ||
			(mousePos.y < boardStartY || boardStartY + m_board->GetBoardHeight() < mousePos.y))
		{
			return false;
		}

		// 보드 좌표로 변환
		boardPos.x = mousePos.x - boardStartX;
		boardPos.y = mousePos.y - boardStartY;
		return true;
	}

	bool BoardManager::GetGridIndexFromBoardPos(const MOUSE_POS& boardPos, GridIndex& gridIndex)
	{
		// 그리드 범위 밖을 클릭했다면 false
		if ((boardPos.x < m_board->GetGridOffsetX() || (m_board->GetGridOffsetX() + m_board->GetGridWidth()) <= boardPos.x) ||
			(boardPos.y < m_board->GetGridOffsetY() || (m_board->GetGridOffsetY()+ m_board->GetGridHeight()) <= boardPos.y))
		{
			return false;
		}

		// 상대 좌표 계산
		int relX = boardPos.x - m_board->GetGridOffsetX();
		int relY = boardPos.y - m_board->GetGridOffsetY();

		// cell + gap = unit 너비, 높이 계산
		int unitWidth = m_board->GetCellWidth() + m_board->GetGridGap();
		int unitHeight = m_board->GetCellHeight() + m_board->GetGridGap();

		// 유닛 내의 간격을 클릭했는지 검사
		if (relX % unitWidth > m_board->GetCellWidth() || relY % unitHeight > m_board->GetCellHeight())
		{
			return false;
		}

		// 상대 좌표 / unityXY ->  셀 인덱스 계산
		int col = relX / unitWidth;
		int row = relY / unitHeight;

		gridIndex.col = col;
		gridIndex.row = row;
		return true;
	}
}
