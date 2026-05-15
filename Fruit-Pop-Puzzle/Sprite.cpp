#include "sprite.h"
#include "NewGameObject.h"
#include "Transform.h"

Sprite::Sprite(NewGameObject* pOwner)
	: Component(pOwner)
	, m_pTransform(nullptr)
{
	m_pTransform = pOwner->GetComponent<Transform>();
}

void Sprite::Render(HDC hdc)
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

	
	const int x = m_pTransform->GetPosition().x - m_pTransform->GetWidth() / 2;
	const int y = m_pTransform->GetPosition().y - m_pTransform->GetHeight() / 2;

	//const int srcX = m_frameXY[m_frameIndex].x;
	//const int srcY = m_frameXY[m_frameIndex].y;

	// 일단 단일 스프라이트 출력을 위해서 둘다 0값으로 설정했다.
	const int srcX = 0;
	const int srcY = 0;

	// hBitmapDC의 srcX srcY위치에서 이미지 사이즈만큼 실제화면의 hdc의 x,y위치에 m_width, m_height 크기로 확대 / 축소해서 그린다.
	// blend 옵션으로 투명한 배경이 깔끔하게 제거된 상태로 출력된다.
	AlphaBlend(hdc, x, y, m_pTransform->GetWidth(), m_pTransform->GetHeight(),
		hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend);

	// 비트맵 핸들 복원
	SelectObject(hBitmapDC, hOldBitmap);
	DeleteDC(hBitmapDC);
}

void Sprite::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
	m_pBitmapInfo = bitmapInfo;
	m_frameWidth = m_pBitmapInfo->GetWidth();
	m_frameHeight = m_pBitmapInfo->GetHeight();
}
