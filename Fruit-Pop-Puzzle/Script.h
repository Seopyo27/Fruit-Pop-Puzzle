#pragma once
#include <string>
#include "Component.h"

class MyFirstWndGame;
class NewGameObject;

// !! gameObject 사라졌을때 대처 해야됨

class Script : public Component
{
protected:
	MyFirstWndGame* m_pGame = nullptr;
public:
	Script(NewGameObject* pOwner, MyFirstWndGame* game) : Component(pOwner), m_pGame(game) {}
	virtual ~Script() {}
	virtual void Start() {}
	virtual void Update(float dT) {}
	virtual void FixedUpdate() {}
	virtual void End() {}

	NewGameObject* Instantiate(const std::string& name, NewGameObject* prefab);
};