#include <unordered_map>
#include <typeindex>

class Component;

class NewGameObject
{
public:
	template <typename T>
	T* AddComponent() 
	{
		// 이미 해당 타입의 컴포넌트가 있는지 확인
		// 없다면 find()는 end()라는 위치를 리턴한다.end() -> 데이터가 끝난 직후의 빈 공간
		if (m_components.find(typeid(T)) != m_components.end())
		{
			.return static_cast<T*>(m_components[typeid(T)]);
		}

		T* newComponent = new T();
		newComponent->SetOwner(this);
		m_components[typeid(T)] = comp;

		return comp;
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

private:
	std::unordered_map<std::type_index, Component*> m_components;
};