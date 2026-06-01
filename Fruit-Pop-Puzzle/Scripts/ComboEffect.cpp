#include "ComboEffect.h"
#include "Core/Scene.h"
#include "Components/Transform.h"
#include "Components/SpriteRenderer.h"
#include "Components/Text.h"
#include <algorithm>

namespace EHEngine
{
	ComboEffect::ComboEffect(GameObject* pOwner, Scene* scene) : Script(pOwner, scene)
	{
		m_isEnabled = false;
	}

	void ComboEffect::Start()
	{
		InitializeRef();
	}

	void ComboEffect::Update(float dT)
	{
		Transform* effectTransform = m_effect->GetComponent<Transform>();
		SpriteRenderer* effectSprite = m_effect->GetComponent<SpriteRenderer>();

		Transform* textTransform = m_comboText->GetComponent<Transform>();
		SpriteRenderer* textSprite = m_comboText->GetComponent<SpriteRenderer>();
		
		m_elapsedTime = min(m_elapsedTime + dT, m_fadeInTime);
		float t = m_elapsedTime / m_fadeInTime;

		if (t >= 1.0f)
		{
			effectTransform->SetPosition(m_endPos);
			effectSprite->SetIsVisible(false);
			textSprite->SetIsVisible(false);

			m_isEnabled = false;
			return;
		}

		// Lerp
		effectSprite->SetAlpha(t);
		textSprite->SetAlpha(t);

		learning::Vector2f pos = m_startPos + (m_endPos - m_startPos) * t;
		effectTransform->SetPosition(pos);
	}

	void ComboEffect::ShowComboEffect(std::string spriteName, ScreenPos startPos, ScreenPos endPos, float fadeInTime, std::string textName)
	{
		InitializeRef();
		if (fadeInTime == 0) return;

		m_isEnabled = true;
		m_elapsedTime = 0.0f;
		m_fadeInTime = fadeInTime;

		m_startPos = startPos.ToVector2<learning::Vector2f>();
		m_endPos = endPos.ToVector2<learning::Vector2f>();

		m_effect->GetComponent<Transform>()->SetPosition(m_startPos);
		if (spriteName == "Animals")
		{
			m_effect->GetComponent<Transform>()->SetWidth(800);
			m_effect->GetComponent<Transform>()->SetHeight(350);
		}
		else
		{
			m_effect->GetComponent<Transform>()->SetWidth(400);
			m_effect->GetComponent<Transform>()->SetWidth(400);
		}
		m_effect->GetComponent<SpriteRenderer>()->SetBitmapInfo(m_scene->GetResourceManager()->GetBitmapInfo(spriteName));
		m_effect->GetComponent<SpriteRenderer>()->SetIsVisible(true);

		m_comboText->GetComponent<SpriteRenderer>()->SetBitmapInfo(m_scene->GetResourceManager()->GetBitmapInfo(textName));
		m_comboText->GetComponent<SpriteRenderer>()->SetIsVisible(true);
	}

	void ComboEffect::InitializeRef()
	{
		if (m_effect == nullptr)
		{
			m_effect = m_scene->FindGameObjectByName("ComboEffect");
		}


		if (m_comboText == nullptr)
		{
			m_comboText = m_scene->FindGameObjectByName("ComboText");
		}

		return;
	}
}
