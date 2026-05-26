#pragma once
#include "Script.h"
#include "Utillity.h"
namespace EHEngine
{
	class Transform;
	class FallingFruit : public Script
	{
	public:
		FallingFruit(GameObject* pOwner, GameApp* game) : Script(pOwner, game) {}

		void Start() override;
		void Update(float dT) override;

		void SetTargetPos(learning::Vector2f targetPos)
		{
			m_targetPos = targetPos;
		}

		void SetGravityScale(float scale)
		{
			m_gravityScale = scale;
		}
	private:
		Transform* m_transform = nullptr;
		learning::Vector2f m_targetPos = { 0.0f, 0.0f }; // 떨어질 최종 위치
		float m_speed = 0; // 현재 속력
		learning::Vector2f m_dir = { 0.0f, 1.0f }; // 떨어지는 방향
		float m_gravity = 9.8f; // 중력 가속도
		float m_gravityScale = 0.001f; // 중력 가속도 스케일
	};
}


