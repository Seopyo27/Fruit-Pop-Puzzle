#include "Text.h"
#include "GameObject.h"
#include "Transform.h"

namespace EHEngine
{
	Text::Text(GameObject* pOwner, GameApp* pGame) : RenderableComponent(pOwner, pGame)
	{
		m_pTransform = pOwner->GetComponent<Transform>();
	}

	void Text::Render(HDC hdc)
	{
		if (m_pTransform == nullptr) return;

		// 텍스트 렌더

		// 글자 배경 투명
		SetBkMode(hdc, TRANSPARENT);
		// 글자 색 지정
		SetTextColor(hdc, m_color);

		// 이전 폰트 저장 및 현재 폰트 선택
		HFONT oldFont = (HFONT)SelectObject(hdc, m_font);
		
		TextOut(hdc,
				m_pTransform->GetPosition().x,
				m_pTransform->GetPosition().y,
				m_str.c_str(),
				m_str.length());

		// 이전 폰트 복구
		SelectObject(hdc, oldFont);
	}
}	