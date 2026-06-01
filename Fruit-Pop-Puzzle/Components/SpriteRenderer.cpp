#include "SpriteRenderer.h"
#include "Core/GameObject.h"
#include "Components/Transform.h"
#include "External/RenderHelp.h"

namespace EHEngine
{
	SpriteRenderer::SpriteRenderer(GameObject* ownerObj, Scene* scene) : RenderableComponent(ownerObj, scene)
	{
		m_pTransform = m_ownerObj->GetComponent<Transform>();
	}

	void SpriteRenderer::Render(HDC hdc)
	{
		if (m_pBitmapInfo == nullptr) return;
		if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;
		

		HDC hBitmapDC = CreateCompatibleDC(hdc);

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());

		// BLENDFUNCTION 설정 (알파 채널 처리)
		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = static_cast<BYTE>(m_alpha * 255.0f);  // 원본 알파 채널 그대로 사용
		blend.AlphaFormat = AC_SRC_ALPHA;

		const int x = m_pTransform->GetWorldPosition().x - m_pTransform->GetWidth() / 2;
		const int y = m_pTransform->GetWorldPosition().y - m_pTransform->GetHeight() / 2;

		// hBitmapDC의 srcX srcY위치에서 이미지 사이즈만큼 실제화면의 hdc의 x,y위치에 m_width, m_height 크기로 확대 / 축소해서 그린다.
		// blend 옵션으로 투명한 배경이 깔끔하게 제거된 상태로 출력된다.
		AlphaBlend(hdc, x, y, m_pTransform->GetWidth(), m_pTransform->GetHeight(),
			hBitmapDC, m_offsetX, m_offsetY, m_spriteWidth, m_spriteHeight, blend);

		// 비트맵 핸들 복원
		SelectObject(hBitmapDC, hOldBitmap);
		DeleteDC(hBitmapDC);
	}

	void SpriteRenderer::SetBitmapInfo(renderHelp::BitmapInfo* bitmapInfo)
	{
		m_pBitmapInfo = bitmapInfo;
		m_spriteWidth = m_pBitmapInfo->GetWidth();
		m_spriteHeight = m_pBitmapInfo->GetHeight();
		m_offsetX = 0;
		m_offsetY = 0;
	}

	void SpriteRenderer::SetBitmapInfo(renderHelp::BitmapInfo* bitmapInfo, int spriteWidth, int spriteHeight, int offsetX, int offsetY)
	{
		m_pBitmapInfo = bitmapInfo;
		m_spriteWidth = spriteWidth;
		m_spriteHeight = spriteHeight;
		m_offsetX = offsetX;
		m_offsetY = offsetY;
	}
}