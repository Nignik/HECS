#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <memory>
#include <typeindex>

#include "Entity.h"
#include "System.h"

namespace Hori
{
	class World
	{
	public:
		World(const World&) = delete;
		World& operator=(const World&) = delete;

		static World& GetInstance()
		{
			static World instance;
			return instance;
		}

		Entity CreateEntity()
		{
			Entity entity(m_nextEntityID++);
			m_entities.push_back(entity);
			return entity;
		}

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

		template<typename T>
		std::vector<std::shared_ptr<T>> GetCompoentsOfType()
		{
			std::vector<std::shared_ptr<T>> comps;
			for (auto& [id, comp] : m_components[std::type_index(typeid(T))])
			{
				comps.push_back(comp);
			}

			return comps;
		}

		void AddSystem(std::shared_ptr<System> system)
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
		World() = default;
		~World() = default;

		int m_nextEntityID = 0;
		std::vector<Entity> m_entities;
		std::vector<std::shared_ptr<System>> m_systems;
		std::unordered_map<std::type_index, std::unordered_map<int, std::shared_ptr<void>>> m_components;

		template<typename T>
		void NotifySystems(Entity entity)
		{
			
		}
	};
}
