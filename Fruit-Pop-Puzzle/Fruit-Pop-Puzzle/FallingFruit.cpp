#include "FallingFruit.h"
#include "GameObject.h"
#include "Transform.h"

namespace EHEngine
{
	void FallingFruit::Start()
	{
		m_transform = m_pOwnerObj->GetComponent<Transform>();
	}

	void FallingFruit::Update(float dt)
	{
		// 가속 적용
		m_speed += m_gravity * m_gravityScale * dt;
		
		float distanceToTarget = m_transform->GetPosition().Distance(m_targetPos);
		float moveDistance = m_speed * dt;

		// 과일이 목적지에 도달
		if (moveDistance >= distanceToTarget)
		{
			m_transform->SetPosition(m_targetPos.x, m_targetPos.y);
			m_speed = 0;
			SetIsEnabled(false);
		}

		// 과일이 목적지에 도달하지 않음
		else
		{
			// 과일 이동
			m_transform->Translate(m_dir * m_speed * dt);
		}
	}
}
