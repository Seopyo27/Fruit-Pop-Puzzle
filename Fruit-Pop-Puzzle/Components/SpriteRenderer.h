#pragma once
#include "Core/RenderableComponent.h"

namespace renderHelp
{
	class BitmapInfo;
}

namespace EHEngine
{
	class Transform;

	class SpriteRenderer : public RenderableComponent
	{
	public:
		SpriteRenderer(GameObject* ownerObj, Scene* scene);

		void Render(HDC hdc) override;

		void SetBitmapInfo(renderHelp::BitmapInfo* bitmapInfo);
		void SetBitmapInfo(renderHelp::BitmapInfo* bitmapInfo, int spriteWidth, int spriteHeight, int offsetX, int offsetY);
		renderHelp::BitmapInfo* GetBitmapInfo() { return m_pBitmapInfo; }

		float GetAlpha(){ return m_alpha; }
		void SetAlpha(float alpha) { m_alpha = alpha; }
		
		void SetSpriteWidth(int width) { m_spriteWidth = width; }
		void SetSpriteHeight(int height) { m_spriteHeight = height; }
		void SetOffsetX(int offsetX) { m_offsetX = offsetX; }
		void SetOffsetY(int offsetY) { m_offsetY = offsetY; }

	private:
		Transform* m_pTransform = nullptr;
		renderHelp::BitmapInfo* m_pBitmapInfo = nullptr;

		float m_alpha = 1.0f;

		// 이미지 사이즈
		int m_spriteWidth = 0;
		int m_spriteHeight = 0;

		// 스프라이트 오프셋
		int m_offsetX = 0;
		int m_offsetY = 0;
	};
}

