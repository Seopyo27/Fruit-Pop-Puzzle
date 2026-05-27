#include "ScoreText.h"
#include "GameObject.h"
#include "GameManager.h"
#include "Text.h"
#include "GameApp.h"
#include <iomanip>
#include <sstream>

namespace EHEngine
{
	void ScoreText::Start()
	{
		m_text = m_pOwnerObj->GetComponent<Text>();
		GameObject* board = m_pGame->FindGameObjectByName("Board");
		GameManager* gm = board->GetComponent<GameManager>();

		if (gm == nullptr) return;

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