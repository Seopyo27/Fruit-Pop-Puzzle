#pragma once
#include "RenderableComponent.h"
#include <string>

namespace renderHelp
{
	class BitmapInfo;
}

namespace EHEngine
{
	class Transform;

	class Text : public RenderableComponent
	{
	public:
		Text(GameObject* pOwner, GameApp* pGame);
		void Render(HDC hdc) override;
		void SetText(std::wstring str) { m_str = str; }
		void SetFont(HFONT font) { m_font = font; }
		void SetColor(COLORREF color) { m_color = color; }

	private:
		Transform* m_pTransform = nullptr;

		HFONT m_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		std::wstring m_str = L"텍스트를 입력해주세요.";
		COLORREF m_color = RGB(255, 255, 255);
	};
}

