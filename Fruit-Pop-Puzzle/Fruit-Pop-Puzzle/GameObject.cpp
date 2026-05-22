#include "GameObject.h"
#include "Transform.h"
#include "GameApp.h"

namespace EHEngine
{
	// 이거이렇게해도됨?
	GameObject::GameObject(GameApp* scene) : m_scene(scene)
	{
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		for (const auto& it : m_components)
		{
			delete it.second;
		}
		m_components.clear();
	}
}