#pragma once

#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>

#include "Entity.h"
#include "Component.h"

namespace Hori
{
	class ComponentManager
	{
	public:
		template<typename T>
		void AddComponent(Entity entity, std::shared_ptr<T> component)
		{
			const std::type_index type = std::type_index(typeid(T));
			m_components[type][entity.GetID()] = component;
		}

		template<typename T>
		std::shared_ptr<T> GetComponent(Entity entity)
		{
			const std::type_index type = std::type_index(typeid(T));
			auto it = m_components[type].find(entity.GetID());
			if (it != m_components[type].end())
			{
				return std::static_pointer_cast<T>(it->second);
			}
			return nullptr;
		}

		template <typename T>
		void RemoveComponent(Entity entity)
		{
			auto& compMap = components[std::type_index(typeid(T))];
			auto it = compMap.find(entity);
			if (it != compMap.end())
			{
				delete it->second;  // Delete the component
				compMap.erase(it);  // Remove from the map
			}
		}

		~ComponentManager()
		{
			for (auto& [type, compMap] : components)
			{
				for (auto& [entity, comp] : compMap)
				{
					delete comp;
				}
			}
		}

	private:
		std::unordered_map<std::type_index, std::unordered_map<int, void*>> m_components;
	};
}