#pragma once
#include <Windows.h>
class NewGameObject;

class Component
{
public:
	Component(NewGameObject* pOwner) : m_pOwner(pOwner){}
	virtual ~Component() {}

	virtual void Initialize(){}

	virtual void Render(HDC hdc) {}

	void SetOwner(NewGameObject* owner) { m_pOwner = owner; }
	NewGameObject* GetOwner() const { return m_pOwner; }

protected:
	NewGameObject* m_pOwner;
};