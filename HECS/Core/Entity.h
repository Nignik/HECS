#pragma once

#include <vector>

namespace Hori
{
	class Entity
	{
	public:
		Entity()
			: m_id(0)
		{}

		std::uint32_t GetID() const { return m_id; }
		bool operator<(const Entity& other) const
		{
			return m_id < other.m_id;
		}
		bool operator==(const Entity& other) const
		{
			return m_id == other.m_id;
		}

	private:
		friend class World;

		Entity(std::uint32_t id) : m_id(id) {}
		std::uint32_t m_id;
	};
}

namespace std
{
	template <>
	struct hash<Hori::Entity>
	{
		std::size_t operator()(const Hori::Entity& entity) const noexcept
		{
			return std::hash<int32_t>()(entity.GetID());
		}
	};
}

