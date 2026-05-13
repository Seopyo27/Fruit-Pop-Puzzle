#include "Collider.h"

bool  learning::Intersect(ColliderCircle lhs, ColliderCircle rhs)
{
	return (rhs.center - lhs.center).LengthSquared() <= pow(lhs.radius + rhs.radius, 2);
}

bool learning::Intersect(ColliderBox lhs, ColliderBox rhs)
{
	//x-direction
	if (lhs.center.x - lhs.halfSize.x > rhs.center.x + rhs.halfSize.x) return false;
	if (lhs.center.x + lhs.halfSize.x < rhs.center.x - rhs.halfSize.x) return false;

	//y-direction
	if (lhs.center.y - lhs.halfSize.y > rhs.center.y + rhs.halfSize.y) return false;
	if (lhs.center.y + lhs.halfSize.y < rhs.center.y - rhs.halfSize.y) return false;

	return true;

}

learning::Vector2f learning::CalRepulsiveForce(ColliderCircle self, ColliderCircle other)
{
	Vector2f v_self(self.center.x, self.center.y);
	Vector2f v_other(other.center.x, other.center.y);

	// 두 원의 거리 구하기, 원 중심 기준
	float distance = v_self.Distance(v_other);

	// 겹친 거리 구하기, [반지름 + 반지름 - distance]
	float interDistance = (self.radius + other.radius) - distance;

	if (interDistance <= 0.0f)
	{
		return { 0, 0 };
	}

	//밀려나는 방향 계산
	Vector2f dir =  v_self - v_other;
	dir.Normalize();

	return dir * interDistance * 0.5;
}