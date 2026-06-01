#include "Text.h"
#include "Core/GameObject.h"
#include "Components/Transform.h"

namespace EHEngine
{
	Text::Text(GameObject* ownerObj, Scene* scene) : RenderableComponent(ownerObj, scene)
	{
		m_pTransform = ownerObj->GetComponent<Transform>();
	}

	void Text::Render(HDC hdc)
	{
		if (m_pTransform == nullptr) return;

		SetBkMode(hdc, m_isVisibleBackGround ? OPAQUE : TRANSPARENT);  // 글자 배경 투명
		SetBkColor(hdc, m_backGroundcolor);
		SetTextColor(hdc, m_color);   // 글자 색 지정
		HFONT oldFont = (HFONT)SelectObject(hdc, m_font);   // 이전 폰트 저장 및 현재 폰트 선택

		TextOut(hdc,
			m_pTransform->GetWorldPosition().x,
			m_pTransform->GetWorldPosition().y,
			m_str.c_str(),
			m_str.length());

		SelectObject(hdc, oldFont);  // 이전 폰트 복구
	}
}