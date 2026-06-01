#pragma once
#include <Windows.h>
#include <typeindex>

namespace EHEngine
{
	class Scene;
	class GameObject;

	class Component
	{
	public:
		Component(GameObject* ownerObj, Scene* scene) : m_ownerObj(ownerObj), m_scene(scene) {}
		virtual ~Component() = default;

		std::type_index GetTypeIndex() { return m_typeIndex; }
		void SetTypeIndex(std::type_index typeIndex) { m_typeIndex = typeIndex; }

		virtual void Initialize() {}

		virtual void Render(HDC hdc) {}

		virtual void Start() {}
		virtual void Update(float dT) {}
		virtual void FixedUpdate() {}

		bool GetIsEnabled() { return m_isEnabled; }
		void SetIsEnabled(bool b) { m_isEnabled = b; }
		bool GetIsStarted() { return m_isStarted; }
		void SetIsStarted(bool b) { m_isStarted = b; }

		void SetOwner(GameObject* ownerObj) { m_ownerObj = ownerObj; }
		GameObject* GetOwner() const { return m_ownerObj; }

	protected:
		std::type_index m_typeIndex = typeid(Component);

		GameObject* m_ownerObj = nullptr;                   // 주인 오브젝트
		Scene* m_scene = nullptr;                           // 현재 씬
		bool m_isEnabled = true;                            // 업데이트 실행 여부
		bool m_isStarted = false;                           // Start() 실행 여부
	};
}