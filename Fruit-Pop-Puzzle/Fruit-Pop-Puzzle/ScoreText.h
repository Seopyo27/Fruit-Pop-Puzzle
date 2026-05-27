#pragma once
#include "Script.h"

namespace EHEngine
{
	class Text;

	class ScoreText : public Script
	{
	public:
		ScoreText(GameObject* pOwner, GameApp* game) : Script(pOwner, game) {}

		void Start() override;
		void ChangeText(int score);


	private:
		Text* m_text = nullptr;
	};
}


