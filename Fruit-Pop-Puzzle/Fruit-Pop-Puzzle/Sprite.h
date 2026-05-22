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

		void Render(HDC hdc) override;
	private:
		Transform* m_pTransform = nullptr;
		renderHelp::BitmapInfo* m_pBitmapInfo = nullptr;
		int m_frameWidth = 0;
		int m_frameHeight = 0;
	};
}


