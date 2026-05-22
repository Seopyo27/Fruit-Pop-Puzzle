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
	public:
		Script(GameObject* pOwner, GameApp* pGame) : Component(pOwner, pGame) {}
		virtual ~Script() {}

		GameObject* Instantiate(const std::string& name, GameObject* prefab);
	};
}

