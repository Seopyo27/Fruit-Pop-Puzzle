#pragma once
#include "Core/Scene.h"
#include <unordered_map>
#include <string>
#include <memory>


namespace EHEngine
{
	class ResourceManager;
	class InputManager;
	class Scene;

	class SceneManager
	{
	public:
		SceneManager(ResourceManager* resourceManager, InputManager* inputManager) : 
			m_resourceManager(resourceManager), m_inputManager(inputManager) {}

		template <typename T>
		void AddScene(const std::string& sceneName)
		{
			if (m_scenes.find(sceneName) != m_scenes.end()) return;

			m_scenes[sceneName] = std::make_unique<T>(sceneName, m_resourceManager, m_inputManager);
		}

		void SetCurrentScene(const std::string& sceneName);
		const std::string& GetCurrentSceneName() const
		{
			return m_currentSceneName;
		}

		void ChangeScene(const std::string& sceneName);

		void LoadData();

		void FixedUpdate();
		void Update(float deltaTime);
		void Render(HDC hDC);

	private:
		Scene* m_currentScene = nullptr;
		std::string m_currentSceneName;
		std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;

		ResourceManager* m_resourceManager = nullptr;
		InputManager* m_inputManager = nullptr;
	};

}


