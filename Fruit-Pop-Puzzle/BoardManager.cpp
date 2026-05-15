#include "BoardManager.h"
#include "NewGameObject.h"
#include "Transform.h"
#include "Board.h"

void BoardManager::Start()
{
	m_transform = m_pOwner->GetComponent<Transform>();
	m_board = new Board();
}

void BoardManager::Update(float dT)
{

}

