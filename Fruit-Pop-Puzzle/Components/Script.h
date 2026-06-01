#pragma once
#include "Core/Component.h"
#include <string>

namespace EHEngine
{
	class GameApp;
	class Component;

	class Script : public Component
	{
	public:
		Script(GameObject* ownerObj, Scene* scene) : Component(ownerObj, scene) {}
	};
}