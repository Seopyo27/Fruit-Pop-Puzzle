#pragma once
#include "Scripts/Board.h"
#include "Scripts/DropFruit.h"

namespace EHEngine
{
	class BoardManager
	{
	public:
		void SetBoard(Board* board)
		{
			m_board = board;
		}

		void InitBoard(BoardLayout layout)
		{
			m_board->InitBoard(layout);
		}

		bool IsAdjacent(const BoardIndex& index1, const BoardIndex& index2)
		{
			return m_board->IsAdjacent(index1, index2);
		}

		bool GetBoardIndexFromScreenPos(const MousePos& screenPos, BoardIndex& index)
		{
			return m_board->GetBoardIndexFromScreenPos(screenPos, index);
		}

		//*************************************************
		// 과일 생성
		//************************************************* 

		void FillBoard()
		{
			for (int row = 0; row < m_board->GetMaxRow(); row++)
			{
				for (int col = 0; col < m_board->GetMaxCol(); col++)
				{
					m_board->CreateFruit({ row, col });
				}
			}
		}

		void FillEmptySpaces()
		{
			for (int row = 0; row < m_board->GetMaxRow(); row++)
			{
				for (int col = 0; col < m_board->GetMaxCol(); col++)
				{
					BoardIndex index = { row, col };
					if (!m_board->ExistFruitAt(index))
					{
						m_board->CreateFruit(index);
					}
				}
			}
		}

		//*************************************************
		// 과일 매칭
		//************************************************* 

		void SwapFruit(const BoardIndex& index1, const BoardIndex& index2)
		{
			m_board->SwapFruit(index1, index2);
		}

		void InitFruitMatchedList()
		{
			m_matchedFruitList.clear();
		}

		bool ExistMatchedFruit()
		{
			return !m_matchedFruitList.empty();
		}

		void DeleteMatchedFruit()
		{
			for (BoardIndex matchedFruit : m_matchedFruitList)
			{
				m_board->DeleteFruit(matchedFruit);
			}
		}

		int GetMatchedFruitCount()
		{
			return m_matchedFruitList.size();
		}

		void FindMatches()
		{
			for (int row = 0; row < m_board->GetMaxRow(); row++)
			{
				for (int col = 0; col < m_board->GetMaxCol(); col++)
				{
					BoardIndex index = { row, col };
					if (m_board->GetFruitAt(index).obj == nullptr) continue;
					FindCrossMatches(index);
					FindBoxMatches(index);
				}
			}
		}

		void FindCrossMatches(const BoardIndex& startIndex)
		{
			if (m_board == nullptr) return;

			BoardIndex dxy[2][2] = { {{-1, 0}, {1, 0}}, {{0, -1}, {0, 1}} };

			int maxRow = m_board->GetMaxRow();
			int maxCol = m_board->GetMaxCol();

			// 가로, 세로 두 번 검사
			for (int i = 0; i < 2; i++)
			{
				std::vector<BoardIndex> checked;
				checked.push_back(startIndex);
				int matchedCount = 1;

				// 양쪽으로 검사
				for (int j = 0; j < 2; j++)
				{
					BoardIndex currentIndex = startIndex;
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

						if (currentFruit.obj == nullptr) break;
						if (startFruit.type != currentFruit.type) break;

						checked.push_back(currentIndex);
						matchedCount += 1;
					}
				}

				if (matchedCount >= 3)
				{
					m_matchedFruitList.insert(checked.begin(), checked.end());
				}
			}
		}

		void FindBoxMatches(const BoardIndex& startIndex)
		{
			if (m_board == nullptr) return;

			int maxRow = m_board->GetMaxRow();
			int maxCol = m_board->GetMaxCol();

			// 시작 지점의 과일 정보 확인
			const Fruit& startFruit = m_board->GetFruitAt(startIndex);
			if (startFruit.obj == nullptr || startFruit.type == FruitType::NONE) return;

			// 2x2 박스를 구성할 수 있는 4가지 방향 오프셋 (좌상, 우상, 좌하, 우하)
			// 각 오프셋은 기준점을 중심으로 함께 검사해야 할 상대 격자 3개의 위치를 나타냅니다.
			BoardIndex boxOffsets[4][3] = {
				{ {-1, -1}, {-1,  0}, { 0, -1} }, // 왼쪽 위로 박스
				{ {-1,  0}, {-1,  1}, { 0,  1} }, // 오른쪽 위로 박스
				{ { 0, -1}, { 1, -1}, { 1,  0} }, // 왼쪽 아래로 박스
				{ { 1,  0}, { 1,  1}, { 0,  1} }  // 오른쪽 아래로 박스
			};

			// 4가지 방향의 2x2 박스를 각각 검사
			for (int i = 0; i < 4; i++)
			{
				std::vector<BoardIndex> checked;
				checked.push_back(startIndex);
				bool isMatch = true;

				for (int j = 0; j < 3; j++)
				{
					BoardIndex targetIndex = startIndex + boxOffsets[i][j];

					// 경계 검사
					if ((targetIndex.row < 0 || maxRow <= targetIndex.row) ||
						(targetIndex.col < 0 || maxCol <= targetIndex.col))
					{
						isMatch = false;
						break;
					}

					// 과일 정보 확인 및 타입 비교
					const Fruit& targetFruit = m_board->GetFruitAt(targetIndex);
					if (targetFruit.obj == nullptr || targetFruit.type != startFruit.type)
					{
						isMatch = false;
						break;
					}

					checked.push_back(targetIndex);
				}

				// 2x2 박스가 완성되면 매칭 리스트에 추가
				if (isMatch)
				{
					m_matchedFruitList.insert(checked.begin(), checked.end());
				}
			}
		}

		//*************************************************
		// 과일 드랍
		//************************************************* 

		void ProcessFruitDrop()
		{
			// 1열, 2열, 3열.... 을 아래쪽->위쪽으로 순회
			for (int col = 0; col < m_board->GetMaxCol(); col++)
			{
				int targetRow = m_board->GetMaxRow() - 1;

				for (int row = m_board->GetMaxRow() - 1; row >= 0; row--)
				{
					// 과일이 있다면 위칸으로 이동
					if (m_board->ExistFruitAt({ row, col }))
					{
						// 과일을 발견한 위치와 타겟 위치가 다르면, 타겟위치로 이동
						if (row != targetRow)
						{
							StartDropFruit({ row, col }, { targetRow, col });
						}

						// 타겟위치 위로 이동
						targetRow--;
					}

					// 과일이 없다면 타겟위치는 올라가지 않는다. 따라서 타겟위치를 저장해놓고 다음 과일이 나오면
					// 그 위치로 이동시킬 수 있다.
				}
			}
		}

		void StartDropFruit(const BoardIndex& from, const BoardIndex& to)
		{
			if (false == m_board->ExistFruitAt(from)) return;
			if (m_board->ExistFruitAt(to)) return;

			// 타겟 위치 계산
			Pos targetPos = m_board->GetCellCenterPos(to);

			// 과일 떨어뜨리기 스크립트 실행
			GameObject* fruit = m_board->GetFruitAt(from).obj;
			DropFruit* dropFruit = fruit->GetComponent<DropFruit>();
			dropFruit->DropTo(targetPos.ToVector2<learning::Vector2f>());

			m_board->MoveFruitTableData(from, to);
		}


		//*************************************************
		// 과일 드랍
		//************************************************* 

		void PlayPopAnimation()
		{
			for (BoardIndex index : m_matchedFruitList)
			{
				Animator* animator = m_board->GetFruitAt(index).obj->GetComponent<Animator>();
				animator->Play();
			}
		}

	private:
		Board* m_board = nullptr;
		std::set<BoardIndex> m_matchedFruitList;
	};
}


