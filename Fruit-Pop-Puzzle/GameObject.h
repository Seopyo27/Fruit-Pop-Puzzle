#pragma once

#include "Utillity.h"
#include <algorithm>
#include "Collider.h"

// [CHECK]. namespace 포함해서 전방 선언해야 함
namespace renderHelp
{
	class BitmapInfo;
}

enum class ObjectType
{
	BOARD,
	FRUIT,
	POINTER
};

constexpr int OBJECT_NAME_LEN_MAX = 15;

class GameObjectBase
{
	using Vector2f = learning::Vector2f;
public:
	GameObjectBase() = delete;
	GameObjectBase(const GameObjectBase&) = delete;

	GameObjectBase(ObjectType type) : m_type(type) {}

	virtual ~GameObjectBase() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(HDC hdc) = 0;

	void SetPosition(float x, float y) { m_pos = { x, y }; }
	void SetDirection(Vector2f dir) { m_dir = dir; }
	void SetPhysicsForce(Vector2f force) { m_physicsForce = force; }
	void SetSpeed(float speed) { m_speed = speed; }
	void SetName(const char* name);

	void SetWidth(int width) { m_width = width; }
	void SetHeight(int height) { m_height = height; }

	void SetVisible(bool isVisible) { m_isVisible = isVisible; }

	ObjectType Type() const { return m_type; }

	const char* GetName() const { return m_name; }

	Vector2f GetPosition() const { return m_pos; }
	Vector2f GetDirection() const { return m_dir; }
	Vector2f GetPhysicsForce() const { return m_physicsForce; }

	float GetSpeed() const { return m_speed; }


protected:

	void Move(float deltaTime)
	{
		m_pos.x += m_dir.x * m_speed * deltaTime;
		m_pos.y += m_dir.y * m_speed * deltaTime;

		// 기존 위치에 물리 법칙에 의한 힘을 더함
		m_pos += m_physicsForce * deltaTime;
		// 다음 fixedupate 전까지 과한 힘 적용을 방지하기 위한, 댐핑
		m_physicsForce *= 0.5f;
	}

protected:
	ObjectType m_type;

	int m_width = 0;
	int m_height = 0;

	Vector2f m_pos = { 0.0f, 0.0f };
	Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)
	Vector2f m_physicsForce = { 0.0f, 0.0f };

	float m_speed = 0.0f; // 속력

	char m_name[OBJECT_NAME_LEN_MAX] = "";

	bool m_isVisible = true;
};

class GameObject : public GameObjectBase
{
	using ColliderCircle = learning::ColliderCircle;
	using ColliderBox = learning::ColliderBox;

	using BitmapInfo = renderHelp::BitmapInfo;

public:
	GameObject(const GameObject&) = delete;
	GameObject(ObjectType type) : GameObjectBase(type) {}
	~GameObject() override;

	learning::Collider* GetCollider();

	void SetBitmapInfo(BitmapInfo* bitmapInfo);

	void Update(float deltaTime) override;
	void Render(HDC hdc) override;

	void synchronizePosition();

	void SetColliderCircle(float radius);
	void SetColliderBox(float halfWidth, float halfHeight);

	// 플레이어와의 충돌처리
	bool isCollidingPlayer;

protected:
	void DrawCollider(HDC hdc);

	void Move(float deltaTime);

	void DrawBitmap(HDC hdc);
	void UpdateFrame(float deltaTime);

	// Bitmap 정보
	BitmapInfo* m_pBitmapInfo = nullptr;

	// 점진적으로 예쁘게 고쳐 보아요.
	struct FrameFPos
	{
		int x;
		int y;
	};
	// 프레임 정보: 왜 14개냐고 물으시면 셌다고 밖에...:)
	FrameFPos m_frameXY[10] = { { 0, 0 }, };
	int m_frameWidth = 0;
	int m_frameHeight = 0;
	int m_frameIndex = 0;
	int m_frameCount = 1; // 프레임 수

	float m_frameTime = 0.0f;
	float m_frameDuration = 35.0f; // 임의 설정

	// Collider
	learning::Collider* m_collider = nullptr;
	
};
