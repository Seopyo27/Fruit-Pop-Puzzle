#pragma once
#include "Components/Script.h"
#include "External/Utillity.h"
#include "Types/Types.h"

namespace EHEngine
{
	class Text;

	class ComboEffect : public Script
	{
	public:
		ComboEffect(GameObject* pOwner, Scene* scene);

		void Start() override;
		void Update(float dT) override;


		void InitializeRef();
		void ShowComboEffect(std::string spriteName, ScreenPos startPos, ScreenPos endPos, float fadeInTime, std::string textName);
	private:
		GameObject* m_effect = nullptr;
		GameObject* m_comboText = nullptr;
	
		float m_elapsedTime = 0.0f;
		float m_fadeInTime = 0.0f;
		learning::Vector2f m_startPos;
		learning::Vector2f m_endPos;
	};
}


