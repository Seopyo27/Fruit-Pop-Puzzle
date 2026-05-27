#pragma once
#include <unordered_map>
#include <typeindex>
#include <string>
#include <type_traits>
#include <algorithm>
#include "RenderableComponent.h"

namespace EHEngine
{
	class GameApp;
	class Component;
	class Transform;
	
	class GameObject
	{
	public:
		GameObject(GameApp* scene);
		~GameObject();

		void SetId(uint64_t id) { m_id = id; }
		uint64_t GetId() { return m_id; }
		void SetName(std::string name) { m_name = name; }
		std::string GetName() { return m_name;  }

		template <typename T>
		T* AddComponent()
		{
			// 이미 해당 타입의 컴포넌트가 있는지 확인
			// 없다면 find()는 end()라는 위치를 리턴한다.end() -> 데이터가 끝난 직후의 빈 공간
			if (m_components.find(typeid(T)) != m_components.end())
			{
				return static_cast<T*>(m_components[typeid(T)]);
			}

			T* newComponent = new T(this, m_scene);
			m_components[typeid(T)] = newComponent;

			// 렌더블 컴포넌트는 렌더링을 위해 따로 관리
			// <type_traits> 타입을 컴파일 타임에 검사
			// constexpr - 컴파일 타임에 계산 가능한 값/ 함수
			if constexpr (std::is_base_of_v<RenderableComponent, T>)
			{
				m_renderableComponents.push_back(newComponent);

				// 순서대로 렌더링을 위해 정렬
				std::sort(m_renderableComponents.begin(),
					m_renderableComponents.end(),
					[](RenderableComponent* a, RenderableComponent* b)
					{
						return a->GetOrderInLayer() < b->GetOrderInLayer();
					});
			}
			return newComponent;
		}

		template <typename T>
		T* GetComponent()
		{
			// find()는 std::unordered_map<std::type_index, Component*>::iterator 를 리턴한다. 
			// 너무길어서 auto로 사용
			auto it = m_components.find(typeid(T));
			if (it == m_components.end())
			{
				return nullptr;
			}

			return static_cast<T*>(it->second);
		}

		const std::unordered_map<std::type_index, Component*>& GetComponents() const
		{
			return m_components;
		}

		const std::vector<RenderableComponent*>& GetRenderableComponents() const
		{
			return m_renderableComponents;
		}

		GameApp* GetScene() const
		{
			return m_scene;
		}

	private:
		uint64_t m_id = 0;
		std::string m_name = "NewObject";
		std::unordered_map<std::type_index, Component*> m_components;
		std::vector<RenderableComponent*> m_renderableComponents;
		GameApp* m_scene = nullptr;
	};


}


