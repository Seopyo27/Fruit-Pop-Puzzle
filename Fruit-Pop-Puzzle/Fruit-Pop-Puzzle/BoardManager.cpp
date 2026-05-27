#include "BoardManager.h"
#include "GameApp.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "FallingFruit.h"
#include "Animator.h"
#include <random>

namespace EHEngine
{
	void BoardManager::InitBoard(const BoardLayout& layout)
	{
		if (m_board != nullptr)
		{
			// 과일 목록 초기화
			for (Fruit& fruit : m_board->GetFruitTable())
			{
				if (fruit.gameObject != nullptr)
				{
					m_pGame->DestroyGameObject(fruit.gameObject->GetId());
				}
			}
			delete m_board;
		}

		m_board = new Board();
		m_board->InitBoard(layout);

		// 매칭된 과일 목록 초기화
		m_fruitMatchedList.clear();
	}

	void BoardManager::CreateFruit(const GridIndex& index)
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

		// Trasnform 추가
		learning::Vector2f cellCenterPos = m_board->GetCellCenterPos(index);
		transform->SetPosition( (m_pOwnerObj->GetComponent<Transform>()->GetPosition().x - m_board->GetBoardWidth() / 2) + cellCenterPos.x,
			                    (m_pOwnerObj->GetComponent<Transform>()->GetPosition().y - m_board->GetBoardHeight() / 2) + cellCenterPos.y );
		transform->SetWidth(80);
		transform->SetHeight(80);

		static const char* fruitSpriteSheetNames[] = { "AppleSpriteSheet", "BananaSpriteSheet", "GrapesSpriteSheet", "WaterMelonSpriteSheet"};

		std::cout << m_pGame->GetBitmapInfo("AppleSpriteSheet") << std::endl;
		// Sprite 추가
		Sprite* sprite = newObj->AddComponent<Sprite>();
		sprite->SetBitmapInfo(m_pGame->GetBitmapInfo(fruitSpriteSheetNames[fruitTypeNum]), 200, 200, 0, 0);
		// 렌더링 순서 보드(0) 다음
		sprite->SetOrderInLayer(1);

		// FallingFruit 추가, 처음엔 움직이지 않음
		newObj->AddComponent<FallingFruit>()->SetIsEnabled(false);

		// Animator 추가
		Animator* animator = newObj->AddComponent<Animator>();
		SpriteSheetLayout sheetLayout =
		{
			2000,
			200,
			0,
			0,
			200,
			200,
			0,
			0,
			1,
			10,
			10
		};

		animator->InitAnimationClip(m_pGame->GetBitmapInfo(fruitSpriteSheetNames[fruitTypeNum]), sheetLayout);
		
		// 과일 구조체 세팅
		Fruit newFruit = { fruitType , newObj };

		// 보드에 과일 데이터 추가
		PlaceFruit(newFruit, index);
	}

	void BoardManager::PlaceFruit(const Fruit& fruit, const GridIndex& index)
	{
		// 놓을 자리에 과일이 있다면 오브젝트 제거
		if (m_board->ExistFruit(index))
		{
			m_pGame->DestroyGameObject(m_board->GetFruitAt(index).gameObject->GetId());
		}

		m_board->PlaceFruit(fruit, index);
	}

	void BoardManager::DeleteFruit(const GridIndex& index)
	{
		const Fruit& fruit = m_board->GetFruitAt(index);

		if (fruit.gameObject == nullptr) return;

		m_pGame->DestroyGameObject(fruit.gameObject->GetId());
		m_board->DeleteFruit(index);
	}

	void BoardManager::RefillAllFruit()
	{
		for (int row = 0; row < m_board->GetMaxRow(); row++)
		{
			for (int col = 0; col < m_board->GetMaxCol(); col++)
			{
				CreateFruit({ row, col });
			}
		}
	}

	// index1과 index2에 과일이 둘다 있을 때, 위치를 스왑
	void BoardManager::SwapFruit(const GridIndex& index1, const GridIndex& index2)
	{
		// 과일 오브젝트 조회
		GameObject* fruit1 = m_board->GetFruitAt(index1).gameObject;
		GameObject* fruit2 = m_board->GetFruitAt(index2).gameObject;

		if (fruit1 == nullptr || fruit2 == nullptr) return;

		// 트랜스폼 조회
		Transform* t1 = fruit1->GetComponent<Transform>();
		Transform* t2 = fruit2->GetComponent<Transform>();

		// 트랜스폼 위치 스왑
		learning::Vector2f tempPos = t1->GetPosition();
		t1->SetPosition(t2->GetPosition().x, t2->GetPosition().y);
		t2->SetPosition(tempPos.x, tempPos.y);

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

	void BoardManager::FindCrossMatches(const GridIndex& startIndex)
	{
		if (m_board == nullptr) return;

		GridIndex dxy[2][2] = { {{-1, 0}, {1, 0}}, {{0, -1}, {0, 1}} };

		int maxRow = m_board->GetMaxRow();
		int maxCol = m_board->GetMaxCol();

		// 가로, 세로 두 번 검사
		for (int i = 0; i < 2; i++)
		{
			std::vector<GridIndex> checked;
			checked.push_back(startIndex);
			int matchedCount = 1;

			// 양쪽으로 검사
			for (int j = 0; j < 2; j++)
			{
				GridIndex currentIndex = startIndex;
				while (true)
				{
					currentIndex = currentIndex + dxy[i][j];

					if ((currentIndex.row < 0 || maxRow <= currentIndex.row) ||
						(currentIndex.col < 0 || maxCol <= currentIndex.col))
					{
						break;
					}

					const Fruit& currentFruit = m_board->GetFruitAt(currentIndex);
					const Fruit& startFruit = m_board->GetFruitAt(startIndex);

					if (currentFruit.gameObject == nullptr) break;
					if (startFruit.type != currentFruit.type) break;

					checked.push_back(currentIndex);
					matchedCount += 1;
				}
			}

			if (matchedCount >= 3)
			{
				m_fruitMatchedList.insert(checked.begin(), checked.end());
			}
		}
	}

	void BoardManager::FindBoxMatches(const GridIndex& startIndex)
	{
		if (m_board == nullptr) return;

		int maxRow = m_board->GetMaxRow();
		int maxCol = m_board->GetMaxCol();

		// 시작 지점의 과일 정보 확인
		const Fruit& startFruit = m_board->GetFruitAt(startIndex);
		if (startFruit.gameObject == nullptr || startFruit.type == FruitType::NONE) return;

		// 2x2 박스를 구성할 수 있는 4가지 방향 오프셋 (좌상, 우상, 좌하, 우하)
		// 각 오프셋은 기준점을 중심으로 함께 검사해야 할 상대 격자 3개의 위치를 나타냅니다.
		GridIndex boxOffsets[4][3] = {
			{ {-1, -1}, {-1,  0}, { 0, -1} }, // 왼쪽 위로 박스
			{ {-1,  0}, {-1,  1}, { 0,  1} }, // 오른쪽 위로 박스
			{ { 0, -1}, { 1, -1}, { 1,  0} }, // 왼쪽 아래로 박스
			{ { 1,  0}, { 1,  1}, { 0,  1} }  // 오른쪽 아래로 박스
		};

		// 4가지 방향의 2x2 박스를 각각 검사
		for (int i = 0; i < 4; i++)
		{
			std::vector<GridIndex> checked;
			checked.push_back(startIndex);
			bool isMatch = true;

			for (int j = 0; j < 3; j++)
			{
				GridIndex targetIndex = startIndex + boxOffsets[i][j];

				// 경계 검사
				if ((targetIndex.row < 0 || maxRow <= targetIndex.row) ||
					(targetIndex.col < 0 || maxCol <= targetIndex.col))
				{
					isMatch = false;
					break;
				}

				// 과일 정보 확인 및 타입 비교
				const Fruit& targetFruit = m_board->GetFruitAt(targetIndex);
				if (targetFruit.gameObject == nullptr || targetFruit.type != startFruit.type)
				{
					isMatch = false;
					break;
				}

				checked.push_back(targetIndex);
			}

			// 2x2 박스가 완성되면 매칭 리스트에 추가
			if (isMatch)
			{
				m_fruitMatchedList.insert(checked.begin(), checked.end());
			}
		}
	}

	void BoardManager::InitFruitMatchedList()
	{
		m_fruitMatchedList.clear();
	}

	bool BoardManager::ExistMatchedFruit()
	{
		return !m_fruitMatchedList.empty();
	}

	void BoardManager::DeleteMatchedFruit()
	{
		// 매칭된 과일 제거
		for (GridIndex matchedFruit : m_fruitMatchedList)
		{
			DeleteFruit(matchedFruit);
		}
	}

	void BoardManager::FillFruitEmptySpaces()
	{
		for (int row = 0; row < m_board->GetMaxRow(); row++)
		{
			for (int col = 0; col < m_board->GetMaxCol(); col++)
			{
				if (!m_board->ExistFruit({row, col}))
				{
					CreateFruit({ row, col });
				}
			}
		}
	}

	void BoardManager::FindMathes()
	{
		for (int row = 0; row < m_board->GetMaxRow(); row++)
		{
			for (int col = 0; col < m_board->GetMaxCol(); col++)
			{
				if (m_board->GetFruitAt({ row, col }).gameObject == nullptr) continue;
				FindCrossMatches({ row, col });
				FindBoxMatches({ row, col });
			}
		}
	}

	// 과일들을 아래 빈공간으로 떨어뜨림
	void BoardManager::CheckDropFruit()
	{
		// 1열, 2열, 3열.... 을 아래쪽->위쪽으로 순회
		for (int col = 0; col < m_board->GetMaxCol(); col++)
		{
			int targetRow = m_board->GetMaxRow() - 1;

			for (int row = m_board->GetMaxRow() - 1; row >= 0; row--)
			{
				// 과일이 있다면 위칸으로 이동
				if (m_board->ExistFruit({ row, col }))
				{
					// 과일을 발견한 위치와 타겟 위치가 다르면, 타겟위치로 이동
					if (row != targetRow)
					{
						DropFruit({ row, col }, { targetRow, col });
					}

					// 타겟위치 위로 이동
					targetRow--;
				}

				// 과일이 없다면 타겟위치는 올라가지 않는다. 따라서 타겟위치를 저장해놓고 다음 과일이 나오면
				// 그 위치로 이동시킬 수 있다.
			}
		}
	}

// from 에서 to로 과일을 떨어드립니다.
// to에 이미 과일이 있다면 오브젝트를 삭제하고 데이터를 덮어씌웁니다.
	void BoardManager::DropFruit(const GridIndex& from, const GridIndex& to)
	{
		// 옮길 과일이 없다면 리턴
		if (!m_board->ExistFruit(from)) return;

		// to에 과일이 있다면 리턴
		if (m_board->ExistFruit(to)) return;
		
		// 타겟 위치 계산
		learning::Vector2f cellCenterPos = m_board->GetCellCenterPos(to); // 그리드 좌표 계산
		learning::Vector2f targetPos = { (m_pOwnerObj->GetComponent<Transform>()->GetPosition().x - m_board->GetBoardWidth() / 2) + cellCenterPos.x, // 월드 좌표 계산
										 (m_pOwnerObj->GetComponent<Transform>()->GetPosition().y - m_board->GetBoardHeight() / 2) + cellCenterPos.y };

		// 과일 떨어뜨리기 스크립트 실행
		GameObject* fruit = m_board->GetFruitAt(from).gameObject;
		FallingFruit* fallingFruit = fruit->GetComponent<FallingFruit>();
		fallingFruit->SetTargetPos(targetPos);
		fallingFruit->SetIsEnabled(true);

		// m_board 과일 데이터 이동
		m_board->MoveFruit(from, to);
	}

	void BoardManager::PrintBoard()
	{
		for (int row = 0; row < m_board->GetMaxRow(); row++)
		{
			for (int col = 0; col < m_board->GetMaxCol(); col++)
			{
				const Fruit& f = m_board->GetFruitAt({ row, col });
				std::cout << "(" << col << ", " << row << ") - [" << int(f.type) << ", " << f.gameObject << "]";
				std::cout << "   ";
			}
			std::cout << std::endl;
		}
	}

	void BoardManager::PrintMatchedList()
	{
		std::cout << "매칭된 과일리스트: ";
		for (GridIndex index : m_fruitMatchedList)
		{
			std::cout << "(" << index.col << ", " << index.row << "), ";
		}
		std::cout << std::endl;
	}

	void BoardManager::PrintFruitBitmap(const GridIndex& index)
	{
		GameObject* obj = m_board->GetFruitAt(index).gameObject;
		if (obj == nullptr) return;
		Sprite* t = obj->GetComponent<Sprite>();
		std::cout << t->GetBitmapInfo() << std::endl;
		
	}

	void BoardManager::PlayAnimation()
	{
		for (GridIndex matchedFruit : m_fruitMatchedList)
		{
			Animator* animator = m_board->GetFruitAt(matchedFruit).gameObject->GetComponent<Animator>();
			animator->Play();
		}
	}
}
