#pragma once
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/Script.h"
#include "Components/Transform.h"
#include "Components/SpriteRenderer.h"
#include "Components/Animator.h"
#include "Scripts/DropFruit.h"
#include "Settings/OrderInLayer.h"
#include "External/Utillity.h"
#include "Types/Types.h"
#include <random>

namespace EHEngine
{
	struct BoardLayout
	{
		int maxRow = 0;
		int maxCol = 0;
		int boardWidth = 0;
		int boardHeight = 0;
		int cellWidth = 0;
		int cellHeight = 0;
		int cellGap = 0;
	};

	struct BoardIndex
	{
		int row;
		int col;

		bool operator<(const BoardIndex& other) const
		{
			if (row != other.row)
				return row < other.row;
			return col < other.col;
		}

		BoardIndex operator+(const BoardIndex& other) const
		{
			return { row + other.row, col + other.col };
		}
	};

	enum class FruitType
	{
		NONE = -1,       
		APPLE,
		BANANA,
		GRAPES,
		WATERMELON,
		COUNT,   // 과일 종류 개수 용도
	};

	struct Fruit
	{
		FruitType type = FruitType::NONE;
		GameObject* obj = nullptr;
	};

	class Board : public Script
	{
	public:
		Board(GameObject* ownerObj, Scene* scene) : Script(ownerObj, scene) {}
		~Board()
		{
			for (Fruit fruit : m_fruitTable)
			{
				if (fruit.obj != nullptr)
				{
					m_scene->DestroyGameObject(fruit.obj->GetId());
				}
			}
		}

		//*************************************************
		// 보드 관리
		//************************************************* 

		void InitBoard(const BoardLayout& layout)
		{
			for (Fruit& fruit : m_fruitTable)
			{
				if (fruit.obj == nullptr) continue;

				SpriteRenderer* sprite = fruit.obj->GetComponent<SpriteRenderer>();
				if (sprite != nullptr)
				{
					sprite->SetIsVisible(false);
				}

				m_scene->DestroyGameObject(fruit.obj->GetId());
			}

			m_maxRow = layout.maxRow;
			m_maxCol = layout.maxCol;
			m_boardWidth = layout.boardWidth;
			m_boardHeight = layout.boardHeight;
			m_cellWidth = layout.cellWidth;
			m_cellHeight = layout.cellHeight;
			m_cellGap = layout.cellGap;
			m_fruitTable.assign(layout.maxRow * layout.maxCol, Fruit{});
		}

		bool IsValidIndex(const BoardIndex& index) const
		{
			return index.row >= 0 && index.row < m_maxRow
				&& index.col >= 0 && index.col < m_maxCol;
		}

		int GetTableIndex(const BoardIndex& index)
		{
			return index.row * m_maxCol + index.col;
		}
		
		Pos GetCellCenterPos(const BoardIndex& index)
		{
			Pos centerPos;
			centerPos.x = 
				(0 - m_boardWidth / 2)
				+ (m_cellWidth / 2)
				+ (index.col * (m_cellWidth + m_cellGap));
			
			centerPos.y =
				(0 - m_boardHeight / 2)
				+ (m_cellHeight / 2)
				+ (index.row * (m_cellHeight + m_cellGap));

			return centerPos;
		}

		bool IsAdjacent(const BoardIndex& index1, const BoardIndex& index2)
		{
			// 인덱스가 보드안에 있는지 확인
			if (false == (IsValidIndex(index1) && IsValidIndex(index2))) return false;

			// 같은 인덱스 인지 확인
			if ((index1.row == index2.row) && (index1.col == index2.col))
			{
				return false;
			}

			return abs(index1.row - index2.row) + abs(index1.col - index2.col) == 1;
		}

		bool GetScreenPosFromBoardIndex(const BoardIndex& index, MousePos& screenPos)
		{
			if (false == IsValidIndex(index)) return false;

			Pos centerPos = GetCellCenterPos(index);
			Transform* transform = m_ownerObj->GetComponent<Transform>();
			int x = (int)transform->GetWorldPosition().x + centerPos.x;
			int y = (int)transform->GetWorldPosition().y + centerPos.y;
			screenPos.x = x;
			screenPos.y = y;
			return true;
		}

		bool GetBoardIndexFromScreenPos(const MousePos& screenPos, BoardIndex& index)
		{
			Transform* transform = m_ownerObj->GetComponent<Transform>();
			learning::Vector2f boardPos = transform->GetWorldPosition();

			float localX = screenPos.x - boardPos.x + m_boardWidth * 0.5f;
			float localY = screenPos.y - boardPos.y + m_boardHeight * 0.5f;

			int strideX = m_cellWidth + m_cellGap;
			int strideY = m_cellHeight + m_cellGap;

			int col = (int)(localX / strideX);
			int row = (int)(localY / strideY);

			if (row < 0 || row >= m_maxRow ||
				col < 0 || col >= m_maxCol)
			{
				return false;
			}

			index.row = row;
			index.col = col;
			return true;
		}

		//*************************************************
		// 과일 생성, 제거
		//************************************************* 

		void CreateFruit(const BoardIndex& index)
		{
			if (ExistFruitAt(index)) return;

			// 과일 타입 랜덤 생성
			static std::random_device rd;
			static unsigned long long seed = rd();
			static std::mt19937 gen(seed);
			static std::uniform_int_distribution<int> dis(0, (int)FruitType::COUNT - 1);

			int fruitTypeNum = dis(gen);
			FruitType fruitType = static_cast<FruitType>(fruitTypeNum);

			// 과일 오브젝트 생성
			GameObject* newFruit = m_scene->CreateGameObject();
			newFruit->SetParent(m_ownerObj);

			// 위치 및 크기 설정
			Transform* transform = newFruit->GetComponent<Transform>();
			
			learning::Vector2f cellCenterPos = GetCellCenterPos(index).ToVector2<learning::Vector2f>();

			transform->SetPosition(cellCenterPos);
			transform->SetWidth(m_cellWidth + 5);
			transform->SetHeight(m_cellHeight + 5);

			// 스프라이트 렌더러 설정
			SpriteRenderer* sprite = newFruit->AddComponent<SpriteRenderer>();

			static const char* fruitSpriteNames[] = { "AppleSpriteSheet", "BananaSpriteSheet", "GrapesSpriteSheet", "WaterMelonSpriteSheet" };

			
			sprite->SetBitmapInfo(m_scene->GetResourceManager()->GetBitmapInfo(fruitSpriteNames[fruitTypeNum]), 200, 200, 0, 0);
			sprite->SetOrderInLayer(OrderInLayer::Fruit);

			// 애니메이터 설정
			Animator* animator = newFruit->AddComponent<Animator>();

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

			animator->InitAnimationClip(m_scene->GetResourceManager()->GetBitmapInfo(fruitSpriteNames[fruitTypeNum]), sheetLayout);
			animator->SetFrameTime(30.0f);

			// 드랍 스크립트 부착
			newFruit->AddComponent<DropFruit>();

			// 테이블 등록
			PlaceFruit({ fruitType, newFruit }, index);
		}

		void PlaceFruit(const Fruit& fruit, const BoardIndex& index)
		{
			if(ExistFruitAt(index)) return;

			int tableIndex = GetTableIndex(index);
			m_fruitTable[tableIndex].type = fruit.type;
			m_fruitTable[tableIndex].obj = fruit.obj;
		}

		Fruit& GetFruitAt(const BoardIndex& index)
		{
			return m_fruitTable[GetTableIndex(index)];
		}

		void DeleteFruit(const BoardIndex& index)
		{
			if (false == ExistFruitAt(index)) return;

			int tableIndex = GetTableIndex(index);
			m_scene->DestroyGameObject(m_fruitTable[tableIndex].obj->GetId());
			m_fruitTable[tableIndex].type = FruitType::NONE;
			m_fruitTable[tableIndex].obj = nullptr;

		}

		bool ExistFruitAt(const BoardIndex& index)
		{
			int tableIndex = GetTableIndex(index);
			return m_fruitTable[tableIndex].type != FruitType::NONE &&
				   m_fruitTable[tableIndex].obj != nullptr;
		}

		void SwapFruit(const BoardIndex& from, const BoardIndex& to)
		{
			if (false == (ExistFruitAt(from) && ExistFruitAt(to))) return;
			
			// 위치 스왑
			Transform* fromT = GetFruitAt(from).obj->GetComponent<Transform>();
			Transform* toT = GetFruitAt(to).obj->GetComponent<Transform>();

			learning::Vector2f tempPos = fromT->GetPosition();
			fromT->SetPosition(toT->GetPosition());
			toT->SetPosition(tempPos);

			// 테이블 스왑
			Fruit tempFruit = m_fruitTable[GetTableIndex(from)];
			m_fruitTable[GetTableIndex(from)] = m_fruitTable[GetTableIndex(to)];
			m_fruitTable[GetTableIndex(to)] = tempFruit;
		}

		void MoveFruit(const BoardIndex& from, const BoardIndex& to)
		{
			if (false == (ExistFruitAt(from) && !ExistFruitAt(to))) return;

			// 위치 이동
			Transform* fromT = GetFruitAt(from).obj->GetComponent<Transform>();
			fromT->SetPosition(GetCellCenterPos(to).ToVector2<learning::Vector2f>());

			// 테이블 이동
			m_fruitTable[GetTableIndex(to)] = m_fruitTable[GetTableIndex(from)];
			m_fruitTable[GetTableIndex(from)] = { FruitType::NONE, nullptr };
		}

		void MoveFruitTableData(const BoardIndex& from, const BoardIndex& to)
		{
			if (false == (ExistFruitAt(from) && !ExistFruitAt(to))) return;

			// 테이블 이동
			m_fruitTable[GetTableIndex(to)] = m_fruitTable[GetTableIndex(from)];
			m_fruitTable[GetTableIndex(from)] = { FruitType::NONE, nullptr };
		}

		int GetMaxRow() const { return m_maxRow; }
		int GetMaxCol() const { return m_maxCol; }
		int GetBoardWidth() const { return m_boardWidth; }
		int GetBoardHeight() const { return m_boardHeight; }
		int GetCellWidth() const { return m_cellWidth; }
		int GetCellHeight() const { return m_cellHeight; }
		int GetCellGap() const { return m_cellGap; }
		
	private:
		int m_maxRow = 0;
		int m_maxCol = 0;
		int m_boardWidth = 0;
		int m_boardHeight = 0;
		int m_cellWidth = 0;
		int m_cellHeight = 0;
		int m_cellGap = 0;

		std::vector<Fruit> m_fruitTable;
	};
}


