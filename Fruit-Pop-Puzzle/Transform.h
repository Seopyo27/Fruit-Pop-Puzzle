#pragma once
#include "Component.h"
#include "Utillity.h"

class Transform : Component
{
public:
	void SetWidth(int width) { m_width = width; }
	void SetHeight(int height) { m_height = height; }
	void SetPosition(float x, float y) { m_pos = { x, y }; }
	void SetDirection(learning::Vector2f dir) { m_dir = dir; }
	void SetSpeed(float speed) { m_speed = speed; }

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	learning::Vector2f GetPosition() const { return m_pos; }
	learning::Vector2f GetDirection() const { return m_dir; }
	float GetSpeed() const { return m_speed; }

	void Move(float deltaTime)
	{
		m_pos.x += m_dir.x * m_speed * deltaTime;
		m_pos.y += m_dir.y * m_speed * deltaTime;
	}
private:
	int m_width = 0; // 너비
	int m_height = 0; // 높이
	learning::Vector2f m_pos = { 0.0f, 0.0f }; // 위치
	learning::Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)
	float m_speed = 0.0f; // 속력
}