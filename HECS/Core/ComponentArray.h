#include <vector>
#include <unordered_map>
#include <cassert>

namespace Hori
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;

		virtual void OnEntityDestroyed(uint32_t entityID) = 0;
		virtual bool CloneComponent(uint32_t srcEntityID, uint32_t dstEntityID) = 0;
	};


	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		// Returns true if insertion was successful and false otherwise
		bool InsertData(std::uint32_t entityID, T component)
		{
			if (HasData(entityID))
				return false;

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
			m_size++;
			return true;
		}
		
		// Returns true there was an entity to remove and false otherwise
		bool RemoveData(std::uint32_t entityID)
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

			m_entityToIndex.erase(entityID);
			m_indexToEntity.erase(indexOfLastElement);

			m_size--;
			return true;
		}
		
		// Returns nullptr if there is no such entity
		T* GetData(std::uint32_t entityID)
		{
			auto it = m_entityToIndex.find(entityID);
			if (it == m_entityToIndex.end())
				return nullptr;

			return &m_components[it->second];
		}

		bool HasData(std::uint32_t entityID)
		{
			return m_entityToIndex.find(entityID) != m_entityToIndex.end();
		}

		void OnEntityDestroyed(std::uint32_t entityID) override
		{
			if (m_entityToIndex.find(entityID) != m_entityToIndex.end())
			{
				RemoveData(entityID);
			}
		}
		
		// returns true if cloning was successful and false otherwise
		bool CloneComponent(std::uint32_t srcEntityID, std::uint32_t dstEntityID) override
		{
			auto it = m_entityToIndex.find(srcEntityID);
			if (it == m_entityToIndex.end())
			{
				return false;
			}

			size_t srcIndex = it->second;
			T componentCopy = m_components[srcIndex];

			InsertData(dstEntityID, componentCopy);
			return true;
		}

	private:
		std::vector<T> m_components{};
		std::unordered_map<std::uint32_t, size_t> m_entityToIndex{}; // Used for accessing components given entity id
		std::unordered_map<size_t, std::uint32_t> m_indexToEntity{}; // Used for keeping components vector dense
		size_t m_size = 0;
	};
}


