#pragma once
#include "Core/Component.h"
#include "Core/RenderableComponent.h"
#include <memory>
#include <unordered_map>
#include <set>
#include <typeindex>
#include <string>
#include <type_traits>
#include <algorithm>


namespace EHEngine
{
	class Scene;
	class Transform;

	class GameObject
	{
	public:
		GameObject(Scene* scene);
		~GameObject();

		template <typename T>
		T* AddComponent()
		{
			if (m_components.find(typeid(T)) != m_components.end())
			{
				return static_cast<T*>(m_components[typeid(T)].get());
			}

			std::unique_ptr<T> newComp = std::make_unique<T>(this, m_scene);
			T* pNewComp = newComp.get();
			newComp->SetTypeIndex(typeid(T));
			m_components[typeid(T)] = std::move(newComp);

			if constexpr (std::is_base_of_v<RenderableComponent, T>)
			{
				m_renderableComponents.push_back(pNewComp);
			}

			return pNewComp;
		}

		template <typename T>
		T* GetComponent()
		{
			auto it = m_components.find(typeid(T));
			if (it == m_components.end())
			{
				return nullptr;
			}

			return static_cast<T*>(m_components[typeid(T)].get());
		}

		void SetId(uint64_t id) { m_id = id; }
		uint64_t GetId() { return m_id; }
		GameObject* GetParent() { return m_parent; }
		void SetParent(GameObject* parent) 
		{
			m_parent = parent;
			parent->AddChild(this);
		}
		void InitParent() { m_parent = nullptr; }
		void AddChild(GameObject* child) { m_children.insert(child); }
		void RemoveChild(GameObject* child) { m_children.erase(child); }
		void SetName(std::string name) { m_name = name; }
		std::string GetName() { return m_name; }

		const auto& GetComponents() const { return m_components;  }
		const auto& GetRenderableComponents() const { return m_renderableComponents; }

		bool GetIsPendingDestroy() { return m_isPendingDestroy; }
		void SetIsPendingDestroy(bool b) { m_isPendingDestroy = b; }

		Scene* GetScene() const
		{
			return m_scene;
		}

	private:
		uint64_t m_id = 0;                 // UUID
		Scene* m_scene = nullptr;          // 현재 씬

		GameObject* m_parent = nullptr;
		std::set<GameObject*> m_children;

		std::string m_name = "NewObject";  // 오브젝트 이름
		bool m_isPendingDestroy = false;   // 지연 삭제 예정 여부

		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;   // 소유 컴포넌트
		std::vector<RenderableComponent*> m_renderableComponents;                       // 소유 컴포넌트 중 렌더링 가능 컴포넌트
	};

}