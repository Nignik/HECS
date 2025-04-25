#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>

namespace Hori
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;

		virtual void OnEntityDestroyed(const uint32_t entityID) = 0;
		virtual bool CloneComponent(const uint32_t srcEntityID, const uint32_t dstEntityID) = 0;
	};

	template<typename T>
	class ComponentArray final : public IComponentArray
	{
	public:
		[[nodiscard]] const uint32_t* Entities()  const
		{
			return m_indexToEntity.data();
		}

		[[nodiscard]]size_t Size() const
		{
			return m_size;
		}

		T* Components()
		{
			return m_components.data();
		}

		[[nodiscard]] bool HasData(const uint32_t entityId) const
		{
			return entityId >= m_size;
		}
		
		// Returns true if insertion was successful and false otherwise
		bool InsertData(const uint32_t entityID, T&& component)
		{
			if (HasData(entityID))
				return false;

			// Insert new component at the end
			size_t newIndex = m_size;
			m_entityToIndex[entityID] = newIndex;
			m_indexToEntity[newIndex] = entityID;
			if (m_components.size() > newIndex)
			{
				m_components[newIndex] = std::move(component);
			}
			else
			{
				m_components.push_back(std::move(component));
			}
			m_size++;
			return true;
		}

		// returns true if cloning was successful and false otherwise
		bool CloneComponent(const uint32_t srcEntityID, const uint32_t dstEntityID) override
		{
			if (srcEntityID >= m_size)
				return false;

			size_t srcIndex = m_entityToIndex[srcEntityID];
			T componentCopy = m_components[srcIndex];

			InsertData(dstEntityID, std::move(componentCopy));
			return true;
		}

		// Returns true there was an entity to remove and false otherwise
		bool RemoveData(const uint32_t entityID)
		{
			if (!HasData(entityID))
				return false;

			// Swap and pop to maintain a packed array
			size_t indexOfRemovedEntity = m_entityToIndex[entityID];
			size_t indexOfLastElement = m_size - 1;
			m_components[indexOfRemovedEntity] = m_components[indexOfLastElement];

			// Update mappings
			std::uint32_t entityOfLastElement = m_indexToEntity[indexOfLastElement];
			m_entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
			m_indexToEntity[indexOfRemovedEntity] = entityOfLastElement;

			m_size--;
			return true;
		}
		
		// Returns nullptr if there is no such entity
		T* GetData(const uint32_t entityId)
		{
			if (entityId >= m_size)
				return nullptr;

			return &m_components[m_entityToIndex[entityId]];
		}

		T* GetComponents()
		{
			return m_components.data();
		}

		std::uint32_t* GetEntities()
		{
			return m_indexToEntity.data();
		}

		
		void OnEntityDestroyed(const uint32_t entityID) override
		{
			if (entityID < m_size)
			{
				RemoveData(entityID);
			}
		}
		
	private:
		std::vector<T> m_components{};
		std::vector<size_t> m_entityToIndex{}; // Used for accessing components given entity id
		std::vector<std::uint32_t> m_indexToEntity{}; // Used for keeping components vector dense
		size_t m_size = 0;
	};
}


