#include "ScoreText.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/Text.h"
#include "Scripts/GameManager.h"
#include <iomanip>
#include <sstream>

namespace EHEngine
{
	void ScoreText::Start()
	{
		m_text = m_ownerObj->GetComponent<Text>();
		GameObject* board = m_scene->FindGameObjectByName("ScoreText");
		GameManager* gm = m_scene->FindGameObjectByName("GameManager")->GetComponent<GameManager>();

		gm->BindScoreChanged(
			[this](int score)
			{
				this->ChangeText(score);
			}
		);


	}

	void ScoreText::ChangeText(int score)
	{
		if (m_text == nullptr) return;

		// 9자리까지 표시, 나머지 0으로 표현 string stream 사용
		std::wstringstream wss;
		wss << std::setw(9) << std::setfill(L'0') << score;

		m_text->SetText(wss.str());
	}
}