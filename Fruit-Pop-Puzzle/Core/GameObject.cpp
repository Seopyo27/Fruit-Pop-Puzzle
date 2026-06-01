#include "GameObject.h"
#include "Core/Scene.h"
#include "Components/Transform.h"

namespace EHEngine
{
	GameObject::GameObject(Scene* scene) : m_scene(scene)
	{
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		if (m_parent)
		{
			m_parent->RemoveChild(this);   // 부모에서 자식 삭제
		}


		for (GameObject* child : m_children)   // 자식들 해제
		{
			child->InitParent();
			m_scene->DestroyGameObject(child->GetId());
		}
	}
}