#include "GameApp.h"

#include "External/NzWndBase.h"
#include "External/GameTimer.h"
#include "Core/RenderContext.h"
#include "Core/SceneManager.h"
#include "Core/ResourceManager.h"
#include "Core/InputManager.h"
#include "Scenes/PlayScene.h"

namespace EHEngine
{
	GameApp::GameApp()
	{
		m_window = std::make_unique<NzWndBase>();
		m_renderContext = std::make_unique<RenderContext>();
		m_resourceManager = std::make_unique<ResourceManager>();
		m_inputManager = std::make_unique<InputManager>();
		m_gameTimer = std::make_unique<GameTimer>();

		m_sceneManager = std::make_unique<SceneManager>(m_resourceManager.get(), m_inputManager.get());
	}

	GameApp::~GameApp() = default;

	bool GameApp::Initialize()
	{
		// 윈도우 생성
		const wchar_t* className = L"GameApp";
		const wchar_t* windowName = L"GameApp";
		if(false == m_window->Create(className, windowName, 800, 800))
		{
			return false;
		}

		// 렌더링 컨택스트 생성
		m_renderContext->InitRenderContext(m_window.get());

		// 리소스 로드
		m_resourceManager->LoadResource();

		// 씬 추가
		m_sceneManager->AddScene<PlayScene>("PlayScene");

		// 시작 씬 설정
		m_sceneManager->ChangeScene("PlayScene");

		// 타이머 초기화
		m_gameTimer->Reset();

		return true;
	}

	void GameApp::Run()
	{
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			m_inputManager->InitInput();

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				m_inputManager->ObserveInput(msg);
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			Update();
			Render();
		}
	}

	void GameApp::Finalize()
	{
		m_window->Destroy();
	}

	void GameApp::Update()
	{
		// 델타 타임 측정
		m_gameTimer->Tick();

		// 가변 프레임
		m_sceneManager->Update(m_fDeltaTime);

		m_fDeltaTime = m_gameTimer->DeltaTimeMS();
		m_fDeltaTimeAccumulator += m_fDeltaTime;

		// 고정 프레임
		while (m_fDeltaTimeAccumulator >= m_fFixedDeltaTime)
		{
			m_sceneManager->FixedUpdate();
			m_fDeltaTimeAccumulator -= m_fFixedDeltaTime;
		}
	}

	void GameApp::Render()
	{
		::PatBlt( m_renderContext->GetBackDC(),
				  0,
				  0,
				  m_window->GetWidth(),
				  m_window->GetHeight(),
				  WHITENESS );

		m_sceneManager->Render(m_renderContext->GetBackDC());

		BitBlt( m_renderContext->GetFrontDC(),
				0,
				0,
				m_window->GetWidth(),
				m_window->GetHeight(),
				m_renderContext->GetBackDC(),
				0,
				0,
				SRCCOPY);
	}

}