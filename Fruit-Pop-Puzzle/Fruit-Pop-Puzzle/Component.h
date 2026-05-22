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

		virtual void Start() {}
		virtual void Update(float dT) {}
		virtual void FixedUpdate() {}

		bool GetIsStarted() { return m_IsStarted; }
		void SetIsStarted(bool b) { m_IsStarted = b; }

		void SetOwner(GameObject* owner) { m_pOwnerObj = owner; }
		GameObject* GetOwner() const { return m_pOwnerObj; }

	protected:
		GameObject* m_pOwnerObj;
		GameApp* m_pGame = nullptr;
		bool m_IsStarted = false;
	};
}

