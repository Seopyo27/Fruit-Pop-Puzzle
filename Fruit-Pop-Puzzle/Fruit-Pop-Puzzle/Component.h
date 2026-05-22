#pragma once
#include <Windows.h>

namespace EHEngine
{
	class GameApp;
	class GameObject;

	class Component
	{
	public:
		Component(GameObject* pOwnerObj, GameApp* pGame) : m_pOwnerObj(pOwnerObj), m_pGame(pGame) {}
		virtual ~Component() {}

		virtual void Initialize() {}

		virtual void Render(HDC hdc) {}

		void SetOwner(GameObject* owner) { m_pOwnerObj = owner; }
		GameObject* GetOwner() const { return m_pOwnerObj; }

	protected:
		GameObject* m_pOwnerObj;
		GameApp* m_pGame = nullptr;
	};
}

