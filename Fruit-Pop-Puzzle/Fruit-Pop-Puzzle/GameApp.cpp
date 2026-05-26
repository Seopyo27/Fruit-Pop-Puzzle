#include "GameApp.h"
#include "GameObject.h"
#include "GameTimer.h"
#include "Utillity.h"
#include "RenderHelp.h"
#include "GameManager.h"

//임시
#include "Sprite.h"
#include "Transform.h"

namespace EHEngine
{
	bool GameApp::Initialize()
	{
		const wchar_t* className = L"GameApp";
		const wchar_t* windowName = L"GameApp";

		// 타이머 초기화
		m_pGameTimer = new GameTimer();
		m_pGameTimer->Reset();

		// 윈도우 생성
		if (false == __super::Create(className, windowName, 800, 800))
		{
			return false;
		}

		// HDC
		RECT rcClient = {};
		GetClientRect(m_hWnd, &rcClient);
		m_width = rcClient.right - rcClient.left;
		m_height = rcClient.bottom - rcClient.top;

		m_hFrontDC = GetDC(m_hWnd);
		m_hBackDC = CreateCompatibleDC(m_hFrontDC);
		m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

		m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

		// 리소스 로드
		LoadResource();

		// 테스트

		// 보드오브젝트 생성 및 위치 조정
		GameObject* obj = CreateGameObject();
		obj->SetName("Board");
		Transform* transform = obj->GetComponent<Transform>();
		transform->SetWidth(800);
		transform->SetHeight(800);
		transform->SetPosition(400, 400);

		// 스프라이트 추가
		Sprite* sprite = obj->AddComponent<Sprite>();
		sprite->SetBitmapInfo(GetBitmapInfo("Board"));

		// 보드 스크립트 테스트
		obj->AddComponent<GameManager>();


		return true;
	}

	void GameApp::Run()
	{
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			// 메세지 후킹
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// 입력 처리
				if (msg.message == WM_LBUTTONDOWN)
				{
					GameApp::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
				}
				else if (msg.message == WM_RBUTTONDOWN)
				{
					GameApp::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
				}
				else if (msg.message == WM_MOUSEMOVE)
				{
					GameApp::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				Update();
				Render();
				m_input.key = Key::NONE;
			}
		}
	}

	void GameApp::Finalize()
	{
		// 타이머 해제
		delete m_pGameTimer;
		m_pGameTimer = nullptr;

		// 윈도우 해제?
		__super::Destroy();

		// 모든 게임 오브젝트 삭제
		for (const auto& it : m_GameObjPtrTable)
		{
			delete it.second;
		}
		m_GameObjPtrTable.clear();

		// 모든 비트맵 인포 삭제
		for (const auto& it : m_pBitmapInfoTable)
		{
			delete it.second;
		}
		m_pBitmapInfoTable.clear();
	}

	void GameApp::Update()
	{
		// 델타 타임 측정
		m_pGameTimer->Tick();

		// 가변 프레임
		LogicUpdate();

		m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
		m_fFrameCount += m_fDeltaTime;

		// 고정 프레임
		while (m_fFrameCount >= 20.0f)
		{
			FixedUpdate();
			m_fFrameCount -= 20.0f;
		}

		DeleteExpiredGameObject();
	}

	// 가변 프레임 루프, m_fDelataTime 사용
	void GameApp::LogicUpdate()
	{
		// 오브젝트 순회
		for (const auto& it : m_GameObjPtrTable)
		{
			GameObject* obj = it.second;
			
			// 오브젝트의 컴포넌트 순회
			for (const auto& jt : obj->GetComponents())
			{
				Component* comp = jt.second;

				// 비활성화라면 실행하지않음
				if (!comp->GetIsEnabled()) continue;

				// Start()가 실행되지 않았다면 1회 실행
				if (!comp->GetIsStarted())
				{
					comp->Start();
					comp->SetIsStarted(true);
				}

				// Update() 실행
				comp->Update(m_fDeltaTime);
			}
		}
	}

	// 고정 프레임 사용
	void GameApp::FixedUpdate()
	{
		// 오브젝트 순회
		for (const auto& it : m_GameObjPtrTable)
		{
			GameObject* obj = it.second;

			// 오브젝트의 컴포넌트 순회
			for (const auto& jt : obj->GetComponents())
			{
				Component* comp = jt.second;

				// 비활성화라면 실행하지않음
				if (!comp->GetIsEnabled()) continue;

				// Start()가 실행되지 않았다면 1회 실행
				if (!comp->GetIsStarted())
				{
					comp->Start();
					comp->SetIsStarted(true);
				}

				// FixedUpdate() 실행
				comp->FixedUpdate();
			}
		}
	}

	void GameApp::Render()
	{
		//Clear the back buffer
		::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

		//메모리 DC에 그리기

		for (const auto& it : m_GameObjPtrTable[1]->GetComponents())
		{
			Component* comp = it.second;
			comp->Render(m_hBackDC);
		}

		for (const auto& it : m_GameObjPtrTable)
		{
			if (it.first == 1) continue;

			GameObject* obj = it.second;

			if (obj == nullptr) continue;

			for(const auto& jt : obj->GetComponents() )
			{
				// Renderalbe 컴포넌트만 분리하는 것 필요.
				Component* comp = jt.second;
				comp->Render(m_hBackDC);
			}
		}

		//메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
		BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
	}

	void GameApp::OnResize(int width, int height)
	{
		std::cout << __FUNCTION__ << std::endl;

		learning::SetScreenSize(width, height);

		__super::OnResize(width, height);

		m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

		HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

		DeleteObject(hPrevBitmap);
	}

	void GameApp::OnClose()
	{
		std::cout << __FUNCTION__ << std::endl;

		SelectObject(m_hBackDC, m_hDefaultBitmap);

		DeleteObject(m_hBackBitmap);
		DeleteDC(m_hBackDC);

		ReleaseDC(m_hWnd, m_hFrontDC);
	}

	void GameApp::OnMouseMove(int x, int y)
	{
		m_MousePosPrev = m_MousePos;
		m_MousePos = { x, y };
		m_input = { Key::MOUSE_MOVING, {x, y} };
	}

	void GameApp::OnLButtonDown(int x, int y)
	{
		m_input = { Key::MOUSE_LEFT_CLICK, {x, y} };
	}

	void GameApp::OnRButtonDown(int x, int y)
	{
		m_input = { Key::MOUSE_RIGHT_CLICK, {x, y} };
	}

	void GameApp::LoadResource()
	{
		AddBitmapInfo("Board", L"./Resource/Board.png");
		AddBitmapInfo("AppleSpriteSheet", L"./Resource/AppleSpriteSheet.png");
		AddBitmapInfo("BananaSpriteSheet", L"./Resource/BananaSpriteSheet.png");
		AddBitmapInfo("GrapesSpriteSheet", L"./Resource/GrapesSpriteSheet.png");
		AddBitmapInfo("WaterMelonSpriteSheet", L"./Resource/WaterMelonSpriteSheet.png");
	}

	GameObject* GameApp::CreateGameObject()
	{
		// 게임 오브젝트 생성 후 등록
		GameObject* newObj = new GameObject(this);
		newObj->SetId(++m_GameObjectCount);
		m_GameObjPtrTable[newObj->GetId()] = newObj;
		return newObj;
	}

	bool GameApp::AddGameObject(GameObject* newObj)
	{
		newObj->SetId(++m_GameObjectCount);
		m_GameObjPtrTable[newObj->GetId()] = newObj;
		return true;
	}

	GameObject* GameApp::GetGameObject(uint64_t id)
	{
		auto it = m_GameObjPtrTable.find(id);

		// 같은 이름을 가진 게임 오브젝트가 없으면 실패
		if (it == m_GameObjPtrTable.end())
		{
			return nullptr;
		}

		return it->second;
	}

	bool GameApp::DestroyGameObject(uint64_t id)
	{
		auto it = m_GameObjPtrTable.find(id);

		// 같은 이름을 가진 게임 오브젝트가 없으면 실패
		if (it == m_GameObjPtrTable.end())
		{
			return false;
		}

		m_expiredGameObjectPtr.push_back(it->second);
		m_GameObjPtrTable.erase(id);
		
		return true;
	}

	void GameApp::DeleteExpiredGameObject()
	{
		for (GameObject* p : m_expiredGameObjectPtr)
		{
			delete p;
		}
		m_expiredGameObjectPtr.clear();
	}

	renderHelp::BitmapInfo* GameApp::AddBitmapInfo(std::string bitMapName, LPCWSTR filename)
	{
		renderHelp::BitmapInfo* bitMapInfo = renderHelp::CreateBitmapInfo(filename);
		if (bitMapInfo == nullptr)
		{
			return nullptr;
		}
		m_pBitmapInfoTable[bitMapName] = bitMapInfo;
		return bitMapInfo;
	}

	renderHelp::BitmapInfo* GameApp::GetBitmapInfo(std::string bitMapName)
	{
		auto it = m_pBitmapInfoTable.find(bitMapName);
		if (it == m_pBitmapInfoTable.end()) return nullptr;
		return it->second;
	}

}


