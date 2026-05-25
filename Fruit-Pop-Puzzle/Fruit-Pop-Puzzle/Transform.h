#pragma once
#include "Component.h"
#include "Utillity.h"

namespace EHEngine
{
	class Transform : public Component
	{
	public:
		Transform(GameObject* owner, GameApp* pGame) : Component(owner, pGame) {}
		
		void SetWidth(int width) { m_width = width; }
		void SetHeight(int height) { m_height = height; }
		void SetPosition(float x, float y) { m_pos = { x, y }; }

		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }
		learning::Vector2f GetPosition() const { return m_pos; }

		void Translate(learning::Vector2f moveVector) { m_pos = m_pos + moveVector; }

	private:
		int m_width = 0; // 너비
		int m_height = 0; // 높이
		learning::Vector2f m_pos = { 0.0f, 0.0f }; // 위치

		//rigidbody에 추가할것들
		//learning::Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)
		//float m_speed = 0.0f; // 속력
	};
}


