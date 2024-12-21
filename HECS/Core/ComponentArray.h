#include <vector>
#include <unordered_map>
#include <cassert>

#include "EntityID.h"

namespace Hori
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;

		virtual void EntityDestroyed(int32_t entityID) = 0;
	};


	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(int32_t entityID, T component)
		{
			assert(m_entityToIndex.find(entityID) == m_entityToIndex.end() && "Component added to same entity more than once.");

			// Insert new component at the end
			size_t newIndex = m_size;
			m_entityToIndex[entityID] = newIndex;
			m_indexToEntity[newIndex] = entityID;
			if (m_components.size() > newIndex)
			{
				m_components[newIndex] = component;
			}
			else
			{
				m_components.push_back(component);
			}
			++m_size;
		}

		void RemoveData(int32_t entityID)
		{
			assert(m_entityToIndex.find(entityID) != m_entityToIndex.end() && "Removing non-existent component.");

			// Swap and pop to maintain a packed array
			size_t indexOfRemovedEntity = m_entityToIndex[entityID];
			size_t indexOfLastElement = m_size - 1;
			m_components[indexOfRemovedEntity] = m_components[indexOfLastElement];

			// Update mappings
			int32_t entityOfLastElement = m_indexToEntity[indexOfLastElement];
			m_entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
			m_indexToEntity[indexOfRemovedEntity] = entityOfLastElement;

			m_entityToIndex.erase(entityID);
			m_indexToEntity.erase(indexOfLastElement);

			--m_size;
		}

		T* GetData(int32_t entityID)
		{
			if (m_entityToIndex.find(entityID) == m_entityToIndex.end())
			{
				return nullptr;
			}
			return &m_components[m_entityToIndex[entityID]];
		}

		bool HasData(int32_t entityID)
		{
			return m_entityToIndex.find(entityID) != m_entityToIndex.end();
		}

		void EntityDestroyed(int32_t entityID) override
		{
			if (m_entityToIndex.find(entityID) != m_entityToIndex.end())
			{
				RemoveData(entityID);
			}
		}

	private:
		std::vector<T> m_components;
		std::unordered_map<int32_t, size_t> m_entityToIndex;
		std::unordered_map<size_t, int32_t> m_indexToEntity;
		size_t m_size = 0;
	};
}


