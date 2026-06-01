#pragma once
#include "Core/Scene.h"
#include "Components/Transform.h"
#include "Components/SpriteRenderer.h"
#include "Components/Text.h"
#include "Settings/OrderInLayer.h"
#include "Scripts/Board.h"
#include "Scripts/BoardManager.h"
#include "Scripts/GameManager.h"
#include "Scripts/CellPointer.h"
#include "Scripts/ScoreText.h"
#include "Scripts/ComboEffect.h"

namespace EHEngine
{
	class PlayScene : public Scene
	{
	public:
		PlayScene(std::string sceneName, ResourceManager* resourceManager, InputManager* inputManager) :
			Scene(sceneName, resourceManager, inputManager) {}

		void LoadData() override
		{
			//*************************************************
			// 보드 오브젝트
			//************************************************* 
 
			// 보드오브젝트 생성 및 위치 조정
			GameObject* boardObj = CreateGameObject();
			boardObj->SetName("Board");
			Transform* transform = boardObj->GetComponent<Transform>();
			transform->SetWidth(800);
			transform->SetHeight(800);
			transform->SetPosition({400, 400});

			// 스프라이트 추가
			SpriteRenderer* sprite = boardObj->AddComponent<SpriteRenderer>();
			sprite->SetBitmapInfo(m_resourceManager->GetBitmapInfo("Board"));

			// 렌더링 순서 제일 먼저
			sprite->SetOrderInLayer(OrderInLayer::Background);

			boardObj->AddComponent<Board>();


			//*************************************************
			// 게임 매니저 오브젝트
			//************************************************* 
			
			GameObject* gmObj = CreateGameObject();
			gmObj->SetName("GameManager");
			gmObj->AddComponent<GameManager>();


			//*************************************************
			// 셀 포인터
			//************************************************* 

			// 셀 포인터 매니저
			GameObject* cpmObj = CreateGameObject();
			cpmObj->SetName("CellPointerManager");
			cpmObj->AddComponent<CellPointer>();

			// 현재 선택 셀 포인터
			GameObject* cPObj = CreateGameObject();
			cPObj->SetName("CurrentCellPointer");
			Transform* cPTransform = cPObj->GetComponent<Transform>();
			cPTransform->SetWidth(75);
			cPTransform->SetHeight(75);
			SpriteRenderer* cPSprite = cPObj->AddComponent<SpriteRenderer>();
			cPSprite->SetBitmapInfo(m_resourceManager->GetBitmapInfo("CurrentPointer"));
			cPSprite->SetOrderInLayer(OrderInLayer::CellPointer);
			cPSprite->SetIsVisible(false);

			// 첫번째 선택 셀 포인터
			GameObject* fPObj = CreateGameObject();
			fPObj->SetName("FirstSelectedCellPointer");
			Transform* fPTransform = fPObj->GetComponent<Transform>();
			fPTransform->SetWidth(75);
			fPTransform->SetHeight(75);
			SpriteRenderer* fPSprite = fPObj->AddComponent<SpriteRenderer>();
			fPSprite->SetBitmapInfo(m_resourceManager->GetBitmapInfo("SelectedPointer"));
			fPSprite->SetOrderInLayer(OrderInLayer::CellPointer);
			fPSprite->SetIsVisible(false);

			// 두번째 선택 셀 포인터
			GameObject* sPObj = CreateGameObject();
			sPObj->SetName("SecondSelectedCellPointer");
			Transform* sPTransform = sPObj->GetComponent<Transform>();
			sPTransform->SetWidth(75);
			sPTransform->SetHeight(75);
			SpriteRenderer* sPSprite = sPObj->AddComponent<SpriteRenderer>();
			sPSprite->SetBitmapInfo(m_resourceManager->GetBitmapInfo("SelectedPointer"));
			sPSprite->SetOrderInLayer(OrderInLayer::CellPointer);
			sPSprite->SetIsVisible(false);


			//*************************************************
			// 스코어 보드
			//************************************************* 
			GameObject* sBObj = CreateGameObject();
			sBObj->SetName("ScoreBoard");
			Transform* sBTransform = sBObj->GetComponent<Transform>();
			sBTransform->SetWidth(400);
			sBTransform->SetHeight(102);
			sBTransform->SetPosition({ 400, 65 });
			SpriteRenderer* sBSprite = sBObj->AddComponent<SpriteRenderer>();
			sBSprite->SetBitmapInfo(m_resourceManager->GetBitmapInfo("ScoreBoard"));
			sBSprite->SetOrderInLayer(OrderInLayer::ScoreBoard);

			GameObject* sTObj = CreateGameObject();
			sTObj->SetName("ScoreText");
			sTObj->SetParent(sBObj);
			Transform* sTTransform = sTObj->GetComponent<Transform>();
			sTTransform->SetPosition({ -80, -15 });
			Text* sTText = sTObj->AddComponent<Text>();
			sTText->SetText(L"000000000");
			sTText->SetFont(
				CreateFont(
					50, 0, 0, 0,
					FW_BOLD,
					FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_DONTCARE,
					L"Arial"
				)
			);
			sTText->SetOrderInLayer(OrderInLayer::ScoreText);
			sTObj->AddComponent<ScoreText>();

			//*************************************************
			// 콤보 이펙트
			//************************************************* 
			GameObject* cemObj = CreateGameObject();
			cemObj->SetName("ComboEffectManager");
			cemObj->AddComponent<ComboEffect>();

			GameObject* cEObj = CreateGameObject();
			cEObj->SetName("ComboEffect");
			Transform* cETransform = cEObj->GetComponent<Transform>();
			cETransform->SetWidth(400);
			cETransform->SetHeight(400);
			SpriteRenderer* cESprite = cEObj->AddComponent<SpriteRenderer>();
			cESprite->SetOrderInLayer(OrderInLayer::ComboEffect);
			cESprite->SetIsVisible(false);

			GameObject* cTObj = CreateGameObject();
			cTObj->SetName("ComboText");
			Transform* cTTransform = cTObj->GetComponent<Transform>();
			cTTransform->SetPosition({ 400, 400 });
			cTTransform->SetWidth(500);
			cTTransform->SetHeight(200);
			SpriteRenderer* cTSprite = cTObj->AddComponent<SpriteRenderer>();
			cTSprite->SetOrderInLayer(OrderInLayer::ComboText);
			cTSprite->SetIsVisible(false);
		}

		void OnEnter() override
		{

		}

		void OnExit() override
		{

		}
	};
}

