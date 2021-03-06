#pragma once

#include "TrioApiRequisites.h"
#include "Object.h"
#include <string>
#include <memory>
#include <vector>
#include "Transform.h"

namespace BrunoEngine
{
	class GameObject : public Object
	{
	public:
		virtual ~GameObject();

		template <class T, typename ...ARGUMENTS> 
		std::shared_ptr<T> AddComponent(ARGUMENTS... args);
		
		template <class T> 
		std::shared_ptr<T> GetComponent() const;

		template <class T> 
		std::vector<std::shared_ptr<T>> GetComponentsInChildren() const;

		int GetLayer() const
		{
			return m_layer;
		}

		const std::shared_ptr<Transform>& GetTransform() const
		{
			return m_transform;
		}

		bool IsActiveInTree() const
		{
			return m_isActiveInTree;
		}

		bool IsActiveSelf() const
		{
			return m_isActiveSelf;
		}

		void SetActiveSelf(bool active);
		void SetLayer(int layer);
		void Update();

		static std::shared_ptr<GameObject> Create(const std::string& name);
		static void Destroy(std::shared_ptr<GameObject>& object);

		friend class Transform;
	private:
		GameObject(const std::string& name);
		void BindComponent(const std::shared_ptr<Component>& component);
		void OnTransformDirty();

		std::vector<std::shared_ptr<Component>> m_components;
		std::vector<std::shared_ptr<Component>> m_addedComponents;
		std::vector<std::shared_ptr<Component>> m_removedComponents;

		std::shared_ptr<Transform> m_transform;
		int m_layer;
		bool m_isActiveSelf;
		bool m_isActiveInTree;
	};

	template <class T, typename ...ARGUMENTS>
	std::shared_ptr<T> GameObject::AddComponent(ARGUMENTS... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>(args...);

		auto isTransform = std::dynamic_pointer_cast<Transform>(component);
		if (m_transform && isTransform)
		{
			return std::shared_ptr<T>();
		}

		m_addedComponents.push_back(component);

		BindComponent(component);

		return component;
	}

	template <class T>
	std::shared_ptr<T> GameObject::GetComponent() const
	{
		for (int i = 0; i < m_addedComponents.size(); ++i)
		{
			auto& component = m_addedComponents[i];
			auto castedComponent = std::dynamic_pointer_cast<T>(component);
			if (castedComponent)
			{
				return castedComponent;
			}
		}

		for (int i = 0; i < m_components.size(); ++i)
		{
			auto& component = m_components[i];
			auto castedComponent = std::dynamic_pointer_cast<T>(component);
			if (castedComponent)
			{
				return castedComponent;
			}
		}

		return std::shared_ptr<T>();
	}

	template <class T>
	std::vector<std::shared_ptr<T>> GameObject::GetComponentsInChildren() const
	{
		std::vector<std::shared_ptr<T>> components;

		for (int i = 0; i < m_addedComponents.size(); ++i)
		{
			auto& component = m_addedComponents[i];
			auto castedComponent = std::dynamic_pointer_cast<T>(component);
			if (castedComponent)
			{
				components.push_back(castedComponent);
			}
		}

		for (int i = 0; i < m_components.size(); ++i)
		{
			auto& component = m_components[i];
			auto castedComponent = std::dynamic_pointer_cast<T>(component);
			if (castedComponent)
			{
				components.push_back(castedComponent);
			}
		}

		int childCount = GetTransform()->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			auto child = this->GetTransform()->GetChild(i);
			std::vector<std::shared_ptr<T>> childComponents = child->GetGameObject()->GetComponentsInChildren<T>();
			for (int j = 0; j < childComponents.size(); ++j)
			{
				components.push_back(childComponents[j]);
			}
		}

		return components;
	}
}
