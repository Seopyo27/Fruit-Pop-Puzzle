#include "INC_Windows.h"

#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include <assert.h>

GameObject::~GameObject()
{
	delete m_collider;
}

void GameObject::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
	assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");

	m_pBitmapInfo = bitmapInfo;

	// 스프라이트 정보는 일단은 하드코딩해요. 
	// 일단, 프레임 크기와 시간이 같다고 가정합니다.
	m_frameWidth = m_pBitmapInfo->GetWidth();
	m_frameHeight = m_pBitmapInfo->GetHeight();
	m_frameIndex = 0;
	m_frameXY[0] = { 0, 0 };
}

void GameObject::Update(float deltaTime)
{
	UpdateFrame(deltaTime);

	Move(deltaTime);

	// Collider 업데이트
	synchronizePosition();
}

void GameObject::Render(HDC hdc)
{
	DrawBitmap(hdc);
	//DrawCollider(hdc);
}


void GameObject::SetColliderCircle(float radius)
{
	if (m_collider)
	{
		delete m_collider;
		m_collider = nullptr;
	}

	m_collider = new learning::ColliderCircle;

	assert(m_collider != nullptr && "Failed to create ColliderCircle!");

	m_collider->center = m_pos;

	static_cast<learning::ColliderCircle*>(m_collider)->radius = radius;
}


void GameObject::SetColliderBox(float width, float height)
{
	if (m_collider)
	{
		delete m_collider;
		m_collider = nullptr;
	}

	m_collider = new learning::ColliderBox;

	assert(m_collider != nullptr && "Failed to create ColliderBox!");

	m_collider->center = m_pos;

	auto* colliderBox = static_cast<learning::ColliderBox*>(m_collider);
	colliderBox->halfSize.x = width / 2.0f;
	colliderBox->halfSize.y = height / 2.0f;
}

learning::Collider* GameObject::GetCollider()
{
	return m_collider;
}

// 과제: 해당 코드의 문제는 무엇일까요? 어떻게 개선하면 좋을까요?
// 개선 방향에 대해 서로 토론하고 비교해 보세요.
void GameObject::DrawCollider(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	m_collider->draw(hdc);

	// 이전 객체 복원 및 펜 삭제
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}

void GameObject::Move(float deltaTime)
{
	GameObjectBase::Move(deltaTime);
}

void GameObject::DrawBitmap(HDC hdc)
{
	if (m_pBitmapInfo == nullptr) return;
	if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

	HDC hBitmapDC = CreateCompatibleDC(hdc);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
	// BLENDFUNCTION 설정 (알파 채널 처리)
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
	blend.AlphaFormat = AC_SRC_ALPHA;

	const int x = m_pos.x - m_width / 2;
	const int y = m_pos.y - m_height / 2;

	const int srcX = m_frameXY[m_frameIndex].x;
	const int srcY = m_frameXY[m_frameIndex].y;

	// hBitmapDC의 srcX srcY위치에서 이미지 사이즈만큼 실제화면의 hdc의 x,y위치에 m_width, m_height 크기로 확대 / 축소해서 그린다.
	// blend 옵션으로 투명한 배경이 깔끔하게 제거된 상태로 출력된다.
	AlphaBlend(hdc, x, y, m_width, m_height,
		hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend);

	// 비트맵 핸들 복원
	SelectObject(hBitmapDC, hOldBitmap);
	DeleteDC(hBitmapDC);
}

void GameObject::UpdateFrame(float deltaTime)
{
	m_frameTime += deltaTime;
	if (m_frameTime >= m_frameDuration)
	{
		m_frameTime = 0.0f;
		m_frameIndex = (m_frameIndex + 1) % (m_frameCount);
	}
}

void GameObject::synchronizePosition()
{
	if (m_collider != nullptr)
	{
		m_collider->center = m_pos;
	}

}

void GameObjectBase::SetName(const char* name)
{
	if (name == nullptr) return;

	strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
	m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}