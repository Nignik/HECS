#pragma once

#include <cstdint>
#include <vector>

#include "Component.h"

namespace Hori
{
	class Entity
	{
	public:
		int32_t GetID() const { return m_id; }

	private:
		friend class World;

		Entity(int32_t id) : m_id(id) {}
		int32_t m_id;
	};

}

