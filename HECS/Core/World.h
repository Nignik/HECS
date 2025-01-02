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
			m_entities.insert(entity.m_id);
			return entity;
		}

		Entity CreatePrototypeEntity()
		{
			Entity entity(m_nextEntityID++);
			m_protypeEntities.insert(entity.m_id);
			return entity;
		}

		void RemoveEntity(Entity entity)
		{
			int32_t entityID = entity.m_id;

			if (m_entities.find(entityID) == m_entities.end())
			{
				return;
			}

			for (auto& [type, arr] : m_componentArrays)
			{
				arr->EntityDestroyed(entityID);
			}

			m_entityComponents.erase(entityID);
			m_entities.erase(entityID);
		}

		

		// Returns pointer to the component if entity has it, otherwise returns nullptr
		template<typename T>
		T* GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity.m_id);
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

		template<typename ... Ts>
		bool HasComponents(Entity entity)
		{
			if ((HasComponent<Ts>(entity.m_id) && ...))
			{
				return true;
			}
			return false;
		}

		template<typename... Ts>
		void AddComponents(Entity entity, Ts&&... components)
		{
			(AddComponent<std::decay_t<Ts>>(entity, std::forward<Ts>(components)), ...);
		}

		template<typename... Ts>
		std::vector<Entity> GetEntitiesWithComponents()
		{
			std::vector<Entity> result;

			for (const auto& entityID : m_entities)
			{
				if ((HasComponent<Ts>(entityID) && ...))
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

		Entity Clone(Entity entity)
		{
			Entity clonedEntity = CreateEntity();

			int32_t srcEntityID = entity.m_id;
			int32_t dstEntityID = clonedEntity.m_id;

			for (auto& componentType : m_entityComponents[entity.m_id])
			{
				auto it = m_componentArrays.find(componentType);
				if (it == m_componentArrays.end())
					continue;

				it->second->CloneComponent(srcEntityID, dstEntityID);

				m_entityComponents[dstEntityID].insert(componentType);
			}

			return clonedEntity;
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

		template<typename T>
		bool HasComponent(int32_t entityID)
		{
			auto arr = GetComponentArray<T>();
			return arr->HasData(entityID);
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			GetComponentArray<T>()->InsertData(entity.m_id, component);
			m_entityComponents[entity.m_id].insert(std::type_index(typeid(T)));
		}

		int32_t m_nextEntityID = 1;
		std::unordered_set<int32_t> m_entities;
		std::unordered_set<int32_t> m_protypeEntities;
		std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_componentArrays;
		std::unordered_map<int32_t, std::unordered_set<std::type_index>> m_entityComponents;
		std::vector<std::unique_ptr<System>> m_systems;

		std::optional<Entity> m_singletonEntity;
	};
}
