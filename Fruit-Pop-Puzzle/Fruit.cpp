#include "Fruit.h"
int Fruit::GetRow() const { return m_row; }
int  Fruit::GetCol() const { return m_col; }
FruitType  Fruit::GetFruitType() const { return m_type; }

void  Fruit::SetRow(int row) { m_row = row; }
void  Fruit::SetCol(int col) { m_col = col; }
void  Fruit::SetFruitType(FruitType type) { m_type = type; }