#pragma once
#include "Utillity.h"
#include "INC_Windows.h"

namespace learning
{
	class Collider
	{
	public:
		Vector2f center;

		virtual void draw(HDC hdc) = 0;
	};

	class ColliderCircle : public Collider
	{
	public:
		double radius;

		void draw(HDC hdc) override
		{
			Ellipse(hdc, center.x - radius,
				center.y - radius,
				center.x + radius,
				center.y + radius);
		}
	};

	class ColliderBox : public Collider
	{
	public:
		Vector2f halfSize;

		void draw(HDC hdc) override
		{
			Rectangle(hdc, center.x - halfSize.x,
				center.y - halfSize.y,
				center.x + halfSize.x,
				center.y + halfSize.y);
		}
	};

	//ColliderBox const& rhs 구조체일 때 이렇게 전달하는이유
	//구조체는 캡슐화 기능이 없기때문에 const
	//구조체는 값 복사 방식으로 전달되기 때문에 참조자 타입

	// Circle
	bool Intersect(ColliderCircle lhs, ColliderCircle rhs);
	// AABB
	bool Intersect(ColliderBox lhs, ColliderBox rhs);

	Vector2f CalRepulsiveForce(ColliderCircle self, ColliderCircle other);
}
