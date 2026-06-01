#pragma once
#include "External/INC_Windows.h"
#include "Core/GameObject.h"
#include "Core/ResourceManager.h"
#include "Core/InputManager.h"
#include <unordered_map>
#include <queue>
#include <memory>

class NzWndBase;

namespace EHEngine
{
	class RenderableComponent;

	class Scene
	{
	public:

		Scene(std::string sceneName, ResourceManager* resourceManager, InputManager* inputManager) : 
			m_sceneName(sceneName), m_resourceManager(resourceManager), m_inputManager(inputManager) {}

		virtual ~Scene() = default;

		//virtual void Initialize(NzWndBase* pWnd) = 0;
		//virtual void Finalize() = 0;

		ResourceManager* GetResourceManager() { return m_resourceManager; }
		InputManager* GetInputManager() { return m_inputManager; }

		virtual void LoadData() = 0;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

		virtual void Update(float deltaTime);
		virtual void FixedUpdate();
		virtual void Render(HDC hDc);
		
		GameObject* CreateGameObject();
		GameObject* GetGameObject(uint64_t id);
		GameObject* FindGameObjectByName(std::string name);
		void DestroyGameObject(uint64_t id);
		void ProcessDestroyGameObjectQueue();

		void AddRenderableComponent(RenderableComponent* component);
		void RemoveRenderableComponent(RenderableComponent* component);

	protected:
		std::string m_sceneName = "Scene";
		ResourceManager* m_resourceManager = nullptr;
		InputManager* m_inputManager = nullptr;

		//*************************************************
		// 게임 오브젝트 관리
		//************************************************* 
		uint64_t m_GameObjectCount = 0;                                            // 오브젝트 개수, 맵 키 값에 사용
		std::unordered_map<uint64_t, std::unique_ptr<GameObject>> m_gameObjects;   // 오브젝트 맵
		std::queue<uint64_t> m_destroyGameObjectQueue;                            // 지연 삭제 오브젝트

		//*************************************************
		// 렌더링 컴포넌트
		//************************************************* 
		std::vector<RenderableComponent*> m_renderableComponents;     // 렌더링 컴포넌트
	};

}