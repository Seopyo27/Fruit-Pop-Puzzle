#pragma once
#include "Components/Script.h"

namespace EHEngine
{
	class Text;

	class ScoreText : public Script
	{
	public:
		ScoreText(GameObject* pOwner, Scene* scene) : Script(pOwner, scene) {}

		void Start() override;
		void ChangeText(int score);

	private:
		Text* m_text = nullptr;
	};

}

