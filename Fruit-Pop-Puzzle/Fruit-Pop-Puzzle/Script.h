#pragma once
#include "Component.h"
#include <string>

// !! gameObject 사라졌을때 대처 해야됨

namespace EHEngine
{
	class GameApp;
	class Component;

	class Script : public Component
	{
	protected:
		bool m_IsStarted = false;
	public:
		Script(GameObject* pOwner, GameApp* pGame) : Component(pOwner, pGame) {}
		virtual ~Script() {}
		virtual void Start() {}
		virtual void Update(float dT) {}
		virtual void FixedUpdate() {}
		virtual void End() {}

		bool GetIsStarted() { return m_IsStarted; }
		void SetIsStarted(bool b) { m_IsStarted = b; }

		GameObject* Instantiate(const std::string& name, GameObject* prefab);
		void Destory();
	};
}

