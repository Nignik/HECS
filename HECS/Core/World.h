#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <memory>
#include <typeindex>
#include <optional>
#include <unordered_set>

#include "ComponentArray.h"
#include "System.h"
#include "Entity.h"
#include "EntityView.h"

namespace Hori
{
	class World
	{
	public:
		World()
		{
			m_singletonEntity = CreateEntity();
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
			m_prototypeEntities.insert(entity.m_id);
			return entity;
		}

		void RemoveEntity(Entity entity)
		{
			int32_t entityID = entity.m_id;

			// it's ok to just call the erases, if the entity doesnt exist nothing will happen
			m_entities.erase(entityID);
			m_prototypeEntities.erase(entityID);
			m_entityComponents.erase(entityID);
			for (auto& [type, arr] : m_componentArrays)
			{
				arr->OnEntityDestroyed(entityID);
			}
		}

		template<typename... Ts>
		void AddComponents(Entity entity, Ts&&... components)
		{
			(AddComponent<std::decay_t<Ts>>(entity, std::forward<Ts>(components)), ...);
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

		template<typename T>
		void AddSingletonComponent(T&& component)
		{
			AddComponent<std::decay_t<T>>(m_singletonEntity.value(), std::forward<T>(component));
		}


		template<typename... Ts>
		std::vector<Entity> GetEntitiesWith()
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

		template<typename... Components>
		EntityView<Components...> GetViewOfEntitiesWith()
		{
			EntityView<Components...> view;

			auto entities = GetEntitiesWith<Components...>();
			for (auto e : entities)
			{
				auto comps = std::make_tuple(GetComponent<Components>(e)...);
				auto entityAndComps = std::tuple_cat(std::make_tuple(e), std::make_tuple(GetComponent<Components>(e))...);
				view.data.push_back(entityAndComps);
			}

			return view;
		}

		template<typename T>
		T* GetComponents()
		{
			return GetComponentArray<T>()->GetComponents();
		}

		// Returns pointer to the component if entity has it, otherwise returns nullptr
		template<typename T>
		T* GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity.m_id);
		}

		// Returns pointer to the signleton component if entity has it, otherwise returns nullptr
		template<typename T>
		T* GetSingletonComponent()
		{
			return GetComponent<T>(m_singletonEntity.value());
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

		template<typename ... Ts>
		bool HasComponents(Entity entity)
		{
			return (HasComponent<Ts>(entity.m_id) && ...);
		}

	private:
		template<typename T>
		ComponentArray<T>* GetComponentArray()
		{
			std::type_index type = std::type_index(typeid(T));
			if (m_componentArrays.find(type) == m_componentArrays.end())
			{
				m_componentArrays.emplace(type, std::make_unique<ComponentArray<T>>());
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
		void AddComponent(Entity entity, T&& component)
		{
			using DT = std::decay_t<T>;
			GetComponentArray<DT>()->InsertData(entity.m_id, std::forward<T>(component));
			m_entityComponents[entity.m_id].insert(std::type_index(typeid(DT)));
		}

		std::uint32_t m_nextEntityID = 1;
		std::unordered_set<std::uint32_t> m_entities{};
		std::unordered_set<std::uint32_t> m_prototypeEntities{}; // prototype entities are meant to not be updated by systems
		std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_componentArrays{};
		std::unordered_map<std::uint32_t, std::unordered_set<std::type_index>> m_entityComponents{};
		std::vector<std::unique_ptr<System>> m_systems{};

		std::optional<Entity> m_singletonEntity{};
	};
}
