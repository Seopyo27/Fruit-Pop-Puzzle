#include "Scene.h"
#include "Core/RenderableComponent.h"
#include <algorithm>

namespace EHEngine
{
	//*************************************************
	// 업데이트
	//************************************************* 

	void Scene::Update(float deltaTime)
	{
		for (const auto& it : m_gameObjects)
		{
			for (const auto& jt : it.second->GetComponents())
			{
				Component* comp = jt.second.get();

				if (false == comp->GetIsEnabled()) continue;
				if (false == comp->GetIsStarted())
				{
					comp->Start();
					comp->SetIsStarted(true);
				}

				comp->Update(deltaTime);
			}
		}

		ProcessDestroyGameObjectQueue();   // 오브젝트 지연삭제 실행
	}

	void Scene::FixedUpdate()
	{
		for (const auto& it : m_gameObjects)
		{
			for (const auto& jt : it.second->GetComponents())
			{
				Component* comp = jt.second.get();

				if (false == comp->GetIsEnabled()) continue;
				if (false == comp->GetIsStarted())
				{
					comp->Start();
					comp->SetIsStarted(true);
				}

				comp->FixedUpdate();
			}
		}
	}



	//*************************************************
	// 렌더링
	//************************************************* 

	void Scene::Render(HDC hDc)
	{
		for (RenderableComponent* rComp : m_renderableComponents)
		{
			if (false == rComp->GetIsVisible()) continue;

			rComp->Render(hDc);
		}
	}



	//*************************************************
	// 게임 오브젝트
	//************************************************* 
	
	GameObject* Scene::CreateGameObject()
	{
		std::unique_ptr<GameObject> newObj = std::make_unique<GameObject>(this);
		GameObject* pNewObj = newObj.get();

		newObj->SetId(++m_GameObjectCount);
		m_gameObjects[newObj->GetId()] = std::move(newObj);

		return pNewObj;

	}

	GameObject* Scene::GetGameObject(uint64_t id)
	{
		return m_gameObjects[id].get();
	}

	GameObject* Scene::FindGameObjectByName(std::string name)
	{
		for (const auto& it : m_gameObjects)
		{
			if (name == it.second->GetName())
			{
				return it.second.get();
			}
		}
		return nullptr;
	}

	void Scene::DestroyGameObject(uint64_t id)
	{
		if (m_gameObjects.find(id) == m_gameObjects.end())
			return;

		m_destroyGameObjectQueue.push(id);
	}

	void Scene::ProcessDestroyGameObjectQueue()
	{
		while (false == m_destroyGameObjectQueue.empty())
		{
			uint64_t id = m_destroyGameObjectQueue.front();
			m_gameObjects[id].reset();
			m_gameObjects.erase(id);
			m_destroyGameObjectQueue.pop();
		}
	}



	//*************************************************
	// 렌더링 컴포넌트
	//************************************************* 

	void Scene::AddRenderableComponent(RenderableComponent* component)
	{
		if (component == nullptr)
			return;

		auto iter = std::find(
			m_renderableComponents.begin(),
			m_renderableComponents.end(),
			component);

		if (iter != m_renderableComponents.end())
			return;

		m_renderableComponents.push_back(component);

		std::sort(
			m_renderableComponents.begin(),
			m_renderableComponents.end(),
			[]( const RenderableComponent* lhs,
				const RenderableComponent* rhs)
			{
	
				return lhs->GetOrderInLayer() < rhs->GetOrderInLayer();
			});
	}

	void Scene::RemoveRenderableComponent(RenderableComponent* component)
	{
		if (component == nullptr)
			return;

		auto iter = std::find(
			m_renderableComponents.begin(),
			m_renderableComponents.end(),
			component);

		if (iter != m_renderableComponents.end())
		{
			m_renderableComponents.erase(iter);
		}
	}
}