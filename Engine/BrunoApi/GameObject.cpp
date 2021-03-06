#include "stdafx.h"
#include "GameObject.h"

#include "Scene.h"

namespace TrioEngine
{
	GameObject::GameObject(const std::string& name) :
		m_layer(0),
		m_isActiveSelf(true),
		m_isActiveInTree(true)
	{
		SetName(name);
	}

	GameObject::~GameObject()
	{
		m_addedComponents.clear();
		m_removedComponents.clear();
		m_components.clear();

		m_transform.reset();
	}

	void GameObject::BindComponent(const std::shared_ptr<Component>& component)
	{
		auto object = Scene::ActiveScene()->GetGameObject(this);
		component->m_ownerObject = object;
	}

	std::shared_ptr<GameObject> GameObject::Create(const std::string& name)
	{
		std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name));

		Scene::ActiveScene()->AddGameObject(object);
		object->m_transform = object->AddComponent<Transform>();

		return object;
	}

	void GameObject::Destroy(std::shared_ptr<GameObject>& object)
	{
		Scene::ActiveScene()->RemoveGameObject(object);
		object.reset();
	}

	void GameObject::OnTransformDirty()
	{
		for (int i = 0; i < m_addedComponents.size(); ++i)
		{
			auto& component = m_addedComponents[i];
			component->OnTransformDirty();
		}

		for (int i = 0; i < m_components.size(); ++i)
		{
			auto& component = m_components[i];
			component->OnTransformDirty();
		}
	}

	void GameObject::SetActiveSelf(bool active)
	{
		m_isActiveSelf = active;
	}

	void GameObject::SetLayer(int layer)
	{
		m_layer = layer;
	}

	void GameObject::Update()
	{
		for (int i = 0; i < m_components.size(); ++i)
		{
			auto& com = m_components[i];
			com->Update();
		}

		do
		{
			std::vector<std::shared_ptr<Component>> added = m_addedComponents;
			m_addedComponents.clear();

			for (int i = 0; i < added.size(); ++i)
			{
				auto& com = added[i];
				com->Update();
				m_components.push_back(com);
			}

			added.clear();

		} while (m_addedComponents.size() > 0);

		for (int i = 0; i < m_removedComponents.size(); ++i)
		{
			auto& com = m_removedComponents[i];
			for (int j = 0; j < m_components.size(); j++)
			{
				if (m_components[j] == com)
				{
					m_components.erase(m_components.begin() + j);
					break;
				}
			}
		}
		m_removedComponents.clear();
	}
}