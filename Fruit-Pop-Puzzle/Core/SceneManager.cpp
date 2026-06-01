#include "SceneManager.h"

namespace EHEngine
{
	void SceneManager::SetCurrentScene(const std::string& sceneName)
	{
		if (m_scenes.find(sceneName) == m_scenes.end()) return;
		m_currentScene = m_scenes[sceneName].get();
	}

	void SceneManager::ChangeScene(const std::string& sceneName)
	{
		auto it = m_scenes.find(sceneName);
		if (it == m_scenes.end()) return;

		if (m_currentScene)
		{
			m_currentScene->OnExit();
		}

		m_currentScene = it->second.get();
		m_currentSceneName = sceneName;

		m_currentScene->LoadData();
		m_currentScene->OnEnter();
	}

	void SceneManager::LoadData()
	{
		m_currentScene->LoadData();
	}

	void SceneManager::FixedUpdate()
	{
		if (!m_currentScene) return;
		m_currentScene->FixedUpdate();
	}

	void SceneManager::Update(float deltaTime)
	{
		if (!m_currentScene) return;
		m_currentScene->Update(deltaTime);
	}

	void SceneManager::Render(HDC hDC)
	{
		if (!m_currentScene) return;
		m_currentScene->Render(hDC);
	}

}