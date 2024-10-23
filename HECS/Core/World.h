#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <memory>

#include "Entity.h"
#include "System.h"
#include "ComponentManager.h"

namespace Hori
{
	class World
	{
	public:
		Entity CreateEntity()
		{
			Entity entity(m_nextEntityID++);
			m_entities.push_back(entity);
			return entity;
		}

		template<typename T>
		void AddComponent(Entity entity, std::shared_ptr<T> component)
		{
			m_componentManager.AddComponent<T>(entity, component);
			NotifySystems<T>(entity);
		}

		template<typename T>
		std::shared_ptr<T> GetComponent(Entity entity)
		{
			return m_componentManager.GetComponent<T>(entity);
		}

		void AddSystem(System* system)
		{
			m_systems.push_back(system);
		}

		void UpdateSystems(float deltaTime)
		{
			for (auto& system : m_systems)
			{
				system->Update(deltaTime);
			}
		}

	private:
		int m_nextEntityID = 0;
		std::vector<Entity> m_entities;
		std::vector<std::shared_ptr<System>> m_systems;
		ComponentManager m_componentManager;

		template<typename T>
		void NotifySystems(Entity entity)
		{
			
		}
	};


}
