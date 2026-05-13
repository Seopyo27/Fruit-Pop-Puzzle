#pragma once
#include "GameObject.h"

enum class FruitType {
	NONE = -1,
	APPLE, // 0
	BANANA, // 1
	GRAPE, // 2
	COUNT // 과일 종류의 개수를 파악하기 위한 용도
};

class Fruit : public GameObject
{
public:
	Fruit(ObjectType type) : GameObject(type) {}
	~Fruit() override = default;

	int GetRow() const;
	int GetCol() const;
	FruitType GetFruitType() const;

	void SetRow(int row);
	void SetCol(int col);
	void SetFruitType(FruitType type);

private:
	int m_row, m_col; // 보드 내 인덱스
	FruitType m_type;
};