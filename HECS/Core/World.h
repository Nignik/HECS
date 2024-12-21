#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <memory>
#include <typeindex>
#include <unordered_set>

#include "ComponentArray.h"
#include "Entity.h"
#include "System.h"
#include "Component.h"

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
			
			if (!instance.m_singletonEntity.has_value())
				instance.m_singletonEntity = instance.CreateEntity();

			return instance;
		}

		Entity CreateEntity()
		{
			Entity entity(m_nextEntityID++);
			m_entities.insert(entity.GetID());
			return entity;
		}

		void RemoveEntity(Entity entity)
		{
			int32_t entityID = entity.GetID();

			if (m_entities.find(entityID) == m_entities.end())
			{
				return;
			}

			for (auto& [type, arr] : m_componentArrays)
			{
				arr->EntityDestroyed(entityID);
			}

			m_entities.erase(entityID);
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			GetComponentArray<T>()->InsertData(entity.GetID(), component);
		}

		// Returns pointer to the component if entity has it, otherwise returns nullptr
		template<typename T>
		T* GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity.GetID());
		}

		template<typename T>
		void AddSingletonComponent(T component)
		{
			GetInstance().AddComponent(m_singletonEntity.value(), component);
		}


		// Returns pointer to the signleton component if entity has it, otherwise returns nullptr
		template<typename T>
		T* GetSingletonComponent()
		{
			return GetInstance().GetComponent<T>(m_singletonEntity.value());
		}

		template<typename T>
		bool HasComponent(int32_t entityID)
		{
			auto arr = GetComponentArray<T>();
			return arr->HasData(entityID);
		}

		template<typename... Components>
		std::vector<Entity> GetEntitiesWithComponents()
		{
			std::vector<Entity> result;

			for (const auto& entityID : m_entities)
			{
				if ((HasComponent<Components>(entityID) && ...))
				{
					result.emplace_back(Entity(entityID));
				}
			}

			return result;
		}

		template<typename T, typename... Args>
		T& AddSystem(Args&&... args)
		{
			static_assert(std::is_base_of<System, T>::value, "T must inherit from System");
			auto system = std::make_unique<T>(std::forward<Args>(args)...);
			T& ref = *system;
			m_systems.push_back(std::move(system));
			return ref;
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

		template<typename T>
		ComponentArray<T>* GetComponentArray()
		{
			std::type_index type = std::type_index(typeid(T));
			if (m_componentArrays.find(type) == m_componentArrays.end())
			{
				m_componentArrays[type] = std::make_shared<ComponentArray<T>>();
			}
			return static_cast<ComponentArray<T>*>(m_componentArrays[type].get());
		}

		int32_t m_nextEntityID = 0;
		std::unordered_set<int32_t> m_entities;
		std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_componentArrays;
		std::vector<std::unique_ptr<System>> m_systems;

		std::optional<Entity> m_singletonEntity;
	};
}
