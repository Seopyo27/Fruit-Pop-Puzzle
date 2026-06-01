#include "DropFruit.h"
#include "Core/GameObject.h"
#include "Components/Transform.h"

namespace EHEngine
{
	DropFruit::DropFruit(GameObject* pOwner, Scene* scene) : Script(pOwner, scene)
	{
		m_isEnabled = false;
	}

	void DropFruit::Start()
	{
		m_transform = m_ownerObj->GetComponent<Transform>();
	}

	void DropFruit::Update(float dt)
	{
		// 가속 적용
		m_speed += m_gravity * m_gravityScale * dt;

		float distanceToTarget = m_transform->GetPosition().Distance(m_targetPos);
		float moveDistance = m_speed * dt;

		// 과일이 목적지에 도달
		if (moveDistance >= distanceToTarget)
		{
			m_transform->SetPosition(m_targetPos);
			m_speed = 0;
			m_isEnabled = false;
		}

		// 과일이 목적지에 도달하지 않음
		else
		{
			// 과일 이동
			m_transform->Translate(m_dir * m_speed * dt);
		}
	}

	void DropFruit::DropTo(learning::Vector2f targetPos)
	{
		m_targetPos = targetPos;
		m_isEnabled = true;
	}
}