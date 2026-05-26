#pragma once
#include "Component.h"

namespace renderHelp
{
	class BitmapInfo;
}
namespace EHEngine
{
	class Transform;

	class Sprite : public Component
	{
	public:
		Sprite(GameObject* pOwner, GameApp* pGame);

		void SetBitmapInfo(renderHelp::BitmapInfo* bitmapInfo);
		renderHelp::BitmapInfo* GetBitmapInfo() { return m_pBitmapInfo; }
		void SetBitmapInfo(renderHelp::BitmapInfo* bitmapInfo, int spriteWidth, int spriteHeight, int offsetX, int offsetY);

		void SetSpriteWidth(int width) { m_spriteWidth = width; }
		void SetSpriteHeight(int height) { m_spriteHeight = height; }
		void SetOffsetX(int offsetX) { m_offsetX = offsetX; }
		void SetOffsetY(int offsetY) { m_offsetY = offsetY; }

		void Render(HDC hdc) override;
	private:
		Transform* m_pTransform = nullptr;
		renderHelp::BitmapInfo* m_pBitmapInfo = nullptr;

		// 이미지 사이즈
		int m_spriteWidth = 0;
		int m_spriteHeight = 0;

		// 스프라이트 오프셋
		int m_offsetX = 0;
		int m_offsetY = 0;
	};
}


