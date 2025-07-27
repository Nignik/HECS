#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <memory>
#include <typeindex>
#include <optional>
#include <ranges>
#include <unordered_set>

#include "ComponentArray.h"
#include "System.h"
#include "Entity.h"

namespace Hori
{
	class World
	{
	public:
		World()
		{
			m_singletonEntity = CreateEntity();
		}

		void World::Destroy()
		{
			std::vector<std::uint32_t> ids;
			ids.reserve(m_entities.size() + m_prototypeEntities.size());
			ids.insert(ids.end(), m_entities.begin(),   m_entities.end());
			ids.insert(ids.end(), m_prototypeEntities.begin(),
									m_prototypeEntities.end());

			if (m_singletonEntity &&
				!m_entities.contains(m_singletonEntity->id) &&
				!m_prototypeEntities.contains(m_singletonEntity->id))
			{
				ids.push_back(m_singletonEntity->id);
			}

			for (std::uint32_t id : ids)
				RemoveEntity(Entity{id});

			m_componentArrays.clear();
			m_systems.clear();

			m_entities.clear();
			m_prototypeEntities.clear();
			m_entityComponents.clear();

			m_singletonEntity.reset();
			m_nextEntityId = 1;
		}

		Entity CreateEntity()
		{
			const Entity entity{m_nextEntityId++};
			m_entities.insert(entity.id);
			return entity;
		}
		
		Entity CreatePrototypeEntity()
		{
			const Entity entity{m_nextEntityId++};
			m_prototypeEntities.insert(entity.id);
			return entity;
		}

		Entity Clone(const Entity entity)
		{
			const Entity clonedEntity = CreateEntity();

			const uint32_t srcEntityID = entity.id;
			const uint32_t dstEntityID = clonedEntity.id;

			for (auto& componentType : m_entityComponents[entity.id])
			{
				auto it = m_componentArrays.find(componentType);
				if (it == m_componentArrays.end())
					continue;

				it->second->CloneComponent(srcEntityID, dstEntityID);

				m_entityComponents[dstEntityID].insert(componentType);
			}

			return clonedEntity;
		}

		template<typename... Ts>
		void AddComponents(const Entity entity, Ts&&... components)
		{
			(AddComponent<std::decay_t<Ts>>(entity, std::forward<Ts>(components)), ...);
		}

		template<typename T, typename... Args>
		T& AddSystem(Args&&... args)
		{
			static_assert(std::is_base_of_v<System, T>, "T must inherit from System");
			m_systems.emplace_back( new T(std::forward<Args>(args)...) );
			return *static_cast<T*>(m_systems.back().get());
		}

		template<typename T>
		void AddSingletonComponent(T&& component)
		{
			AddComponent<std::decay_t<T>>(m_singletonEntity.value(), std::forward<T>(component));
		}

		void RemoveEntity(const Entity entity)
		{
			// it's ok to just call the erases, if the entity doesn't exist, nothing will happen
			m_entities.erase(entity.id);
			m_prototypeEntities.erase(entity.id);
			m_entityComponents.erase(entity.id);
			for (const auto& arr : m_componentArrays | std::views::values)
			{
				arr->RemoveData(entity.id);
			}
		}

		void UpdateSystems(const float deltaTime) const
		{
			for (auto& system : m_systems)
			{
				system->Update(deltaTime);
			}
		}

		template<typename Driver, typename... Rest, typename F>
		void Each(F&& f)
		{
			auto& driverArr = GetComponentArray<Driver>();

			const std::size_t count     = driverArr.Size();
			const auto*       entities  = driverArr.Entities();
			Driver*           comps0    = driverArr.Components();

			for (std::size_t i = 0; i < count; ++i)
			{
				Entity e{ entities[i] };

				if (!(HasComponent<Rest>(e) && ...))
					continue;

				f(e, comps0[i], *GetComponent<Rest>(e)...);
			}
		}

		template<typename T>
		ComponentArray<T>& GetComponentArray()
		{
			auto& basePtr = m_componentArrays[typeid(T)];
			if (!basePtr)
				basePtr = std::make_unique<ComponentArray<T>>();

			return static_cast<ComponentArray<T>&>(*basePtr);
		}

		// Returns pointer to the component if entity has it, otherwise returns nullptr
		template<typename T>
		T* GetComponent(Entity entity)
		{
			return GetComponentArray<T>().GetData(entity.id);
		}

		// Returns pointer to the singleton component if entity has it, otherwise returns nullptr
		template<typename T>
		T* GetSingletonComponent()
		{
			return GetComponent<T>(m_singletonEntity.value());
		}

		template<typename ... Ts>
		bool HasComponents(const Entity entity)
		{
			return (HasComponent<Ts>(entity) && ...);
		}

	private:
		template<typename T>
		void AddComponent(Entity entity, T&& component)
		{
			using DT = std::decay_t<T>;
			GetComponentArray<DT>().InsertData(entity.id, std::forward<T>(component));
			m_entityComponents[entity.id].insert(std::type_index(typeid(DT)));
		}

		template<typename T>
		bool HasComponent(const Entity entity)
		{
			return GetComponentArray<T>().HasData(entity.id);
		}
		
		std::uint32_t m_nextEntityId = 1;
		std::unordered_set<std::uint32_t> m_entities{};
		std::unordered_set<std::uint32_t> m_prototypeEntities{}; // prototype entities are meant to not be updated by systems
		std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_componentArrays{};
		std::unordered_map<std::uint32_t, std::unordered_set<std::type_index>> m_entityComponents{};
		std::vector<std::unique_ptr<System>> m_systems{};

		std::optional<Entity> m_singletonEntity{};
	};
}
