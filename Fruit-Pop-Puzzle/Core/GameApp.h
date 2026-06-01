#pragma once
#include <memory>

class NzWndBase;

class GameTimer;

namespace EHEngine
{
	class RenderContext;
	class SceneManager;
	class ResourceManager;
	class InputManager;
	
	class GameApp
	{
	public:
		// 필수 매니저 할당
		GameApp();
		~GameApp();

		bool Initialize();
		void Run();
		void Finalize();

		void Update();
		void Render();
	private:
		std::unique_ptr<NzWndBase> m_window;
		std::unique_ptr<RenderContext> m_renderContext;
		std::unique_ptr<SceneManager> m_sceneManager;
		std::unique_ptr<ResourceManager> m_resourceManager;
		std::unique_ptr<InputManager> m_inputManager;

		//*************************************************
		//타이머
		//************************************************* 
		std::unique_ptr<GameTimer> m_gameTimer;
		float m_fDeltaTime = 0.0f;
		float m_fDeltaTimeAccumulator = 0.0f;
		float m_fFixedDeltaTime = 20.0f;


	};
}


