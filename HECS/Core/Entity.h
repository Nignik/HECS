#pragma once

#include <cstdint>
#include <vector>

#include "Component.h"

namespace Hori
{
	class Entity
	{
	public:
		Entity()
			: m_id(0)
		{}

		int32_t GetID() const { return m_id; }
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

		Entity(int32_t id) : m_id(id) {}
		int32_t m_id;
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

