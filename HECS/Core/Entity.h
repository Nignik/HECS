#pragma once

#include <vector>

namespace Hori
{
	// Entity with id 0 is the default, and treated as invalid
	struct Entity
	{
	public:
		[[nodiscard]] bool Valid() const { return id != 0; }
		bool operator<(const Entity& other) const
		{
			return id < other.id;
		}
		bool operator==(const Entity& other) const
		{
			return id == other.id;
		}

		uint32_t id = 0;
	};
}

namespace std
{
	template <>
	struct hash<Hori::Entity>
	{
		std::size_t operator()(const Hori::Entity& entity) const noexcept
		{
			return std::hash<uint32_t>()(entity.id);
		}
	};
}

