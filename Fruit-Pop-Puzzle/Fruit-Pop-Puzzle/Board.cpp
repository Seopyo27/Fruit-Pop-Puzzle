#include "Board.h"
#include "Utillity.h"
#include "GameObject.h"
#include "Transform.h"
#include <random>

namespace EHEngine
{
	int Board::GetRowColToIndex(const GridIndex& index)
	{
		return (index.row * m_maxCol) + index.col;
	}

	learning::Vector2f Board::GetCellCenterPos(const GridIndex& index)
	{
		learning::Vector2f cellCenterPos;

		cellCenterPos.x = m_gridOffsetX + (m_cellWidth / 2) + (index.col * (m_cellWidth + m_gridGap));
		cellCenterPos.y = m_gridOffsetY + (m_cellHeight / 2) + (index.row * (m_cellHeight + m_gridGap));

		return cellCenterPos;
	}

	void Board::InitBoard(const BoardLayout& layout)
	{
		// 레이아웃 설정
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

		// 과일 목록 초기화
		for (Fruit fruit : m_fruitTable)
		{
			fruit.type = FruitType::NONE;

			if (fruit.gameObject != nullptr)
			{
				delete fruit.gameObject;
			}
		}

		m_fruitTable.assign(m_maxRow * m_maxCol, Fruit{});

		// 매칭된 과일 목록 초기화
		m_fruitMatchedList.clear();
	}

	// 해당 위치에 과일 배치
	void Board::PlaceFruit(const Fruit& fruit, const GridIndex& index)
	{
		Fruit& targetCell = m_fruitTable[GetRowColToIndex(index)];

		targetCell.type = FruitType::NONE;
		if (targetCell.gameObject != nullptr)
		{
			delete targetCell.gameObject;
			targetCell.gameObject = nullptr;
		}

		targetCell = fruit;
	}

	const Fruit& Board::GetFruitAt(const GridIndex& index)
	{
		return m_fruitTable[GetRowColToIndex(index)];
	}

	void Board::SwapFruit(const GridIndex& i1, const GridIndex& i2)
	{
		GameObject* fruit1 = m_fruitTable[GetRowColToIndex(i1)].gameObject;
		GameObject* fruit2 = m_fruitTable[GetRowColToIndex(i2)].gameObject;

		// 오브젝트 위치 스왑
		Transform* t1 = fruit1->GetComponent<Transform>();
		Transform* t2 = fruit2->GetComponent<Transform>();

		learning::Vector2f tempPos = t1->GetPosition();
		t1->SetPosition(t2->GetPosition().x, t2->GetPosition().y);
		t2->SetPosition(tempPos.x, tempPos.y);

		// 보드 인덱스 스왑
		Fruit tempFruit = m_fruitTable[GetRowColToIndex(i1)];
		m_fruitTable[GetRowColToIndex(i1)] = m_fruitTable[GetRowColToIndex(i2)];
		m_fruitTable[GetRowColToIndex(i2)] = tempFruit;
	}

	bool  Board::IsAdjacent(const GridIndex& index1, const GridIndex& index2)
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

	void Board::FIndBoxMatches(const GridIndex& startIndex)
	{
		GridIndex dxy[4][3] = { {{-1, 0}, {0, 1}, {-1, 1}}, {{1, 0}, {0, 1}, {1, 1}},{{1, 0}, {0, -1}, {1, -1}}, {{-1, 0}, {0, -1}, {-1, -1}} };

		for (int i = 0; i < 4; i++)
		{
			// 확인 한 곳 저장
			std::vector<GridIndex> checked;
			// 시작 지점 저장
			checked.push_back(startIndex);
			// 같은 과일 수
			int matchedCount = 1;

			for (int j = 0; j < 3; j++)
			{
				GridIndex currentIndex = startIndex + dxy[i][j];

				if ((currentIndex.row < 0 || m_maxRow <= currentIndex.row) ||
					(currentIndex.col < 0 || m_maxCol <= currentIndex.col))
				{
					break;
				}

				if (GetFruitAt(currentIndex) == nullptr) break;

				if (GetFruitAt(startIndex)->GetFruitType() != GetFruitAt(currentIndex)->GetFruitType())
				{
					break;
				}

				checked.push_back(currentIndex);
				matchedCount += 1;
			}

			if (matchedCount >= 4)
			{
				m_fruitMatchedList.insert(checked.begin(), checked.end());
				checked.clear();
			}
		}
	}

}