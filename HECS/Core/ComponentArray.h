#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>
#include <limits>

namespace Hori
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;

		virtual bool RemoveData(const uint32_t entityID) = 0;
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
			return entityId < m_entityToIndex.size() && m_entityToIndex[entityId] < m_size;
		}
		
		// Returns true if insertion was successful and false otherwise
		bool InsertData(const uint32_t entityID, T&& component)
		{
			if (HasData(entityID))
				return false;

			// Grow the vector
			size_t newIndex = m_size;
			if (entityID >= m_entityToIndex.size())
				m_entityToIndex.resize(entityID + 1, std::numeric_limits<size_t>::max());

			if (newIndex >= m_indexToEntity.size())
				m_indexToEntity.resize(newIndex + 1);

			// Insert new component at the end
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
		bool RemoveData(const uint32_t entityID) override
		{
			if (!HasData(entityID))
				return false;

			const size_t idxRemoved = m_entityToIndex[entityID];
			const size_t idxLast    = m_size - 1;

			if (idxRemoved != idxLast)
			{
				m_components[idxRemoved]  = std::move(m_components[idxLast]);
				uint32_t entLast          = m_indexToEntity[idxLast];
				m_entityToIndex[entLast]  = idxRemoved;
				m_indexToEntity[idxRemoved] = entLast;
			}

			m_entityToIndex[entityID] = std::numeric_limits<size_t>::max();
			m_size--;
			return true;
		}
		
		// Returns nullptr if there is no such entity
		T* GetData(const uint32_t entityId)
		{
			if (entityId >= m_entityToIndex.size())
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

	private:
		std::vector<T> m_components{};
		std::vector<size_t> m_entityToIndex{}; // Used for accessing components given entity id
		std::vector<std::uint32_t> m_indexToEntity{}; // Used for keeping components vector dense
		size_t m_size = 0;
	};
}


